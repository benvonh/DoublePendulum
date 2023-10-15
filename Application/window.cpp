#include "window.hpp"

#define ThrowRuntime(brief, detail) \
	throw std::runtime_error(std::string(brief) + "\n\n(" + std::string(detail) + ')')

Window::Window()
	: m_Width{}, m_Height{}, m_CentreX{}, m_CentreY{},
	m_Step{false}, m_Quit{false}, m_Pause{false}, m_Robot{}
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ThrowRuntime("Failed to initialise SDL video.", SDL_GetError());

	if (TTF_Init() < 0)
		ThrowRuntime("Failed to initialise TTF extension.", TTF_GetError());

	m_Window = SDL_CreateWindow(
		"Double Pendulum",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		cfg::win::DEFAULT_WIDTH,
		cfg::win::DEFAULT_HEIGHT,
		SDL_WINDOW_RESIZABLE
	);

	if (m_Window == nullptr)
		ThrowRuntime("Failed to create SDL window.", SDL_GetError());

	m_Renderer = SDL_CreateRenderer(
		m_Window, -1,
		SDL_RENDERER_ACCELERATED
	);

	if (m_Renderer == nullptr)
		ThrowRuntime("Failed to create SDL renderer.", SDL_GetError());

	SDL_RWops* font_rw = SDL_RWFromConstMem(
		CaskaydiaCoveNerdFont_Regular_ttf,
		CaskaydiaCoveNerdFont_Regular_ttf_len
	);

	if (font_rw == nullptr)
		ThrowRuntime("Failed to load font from memory.", SDL_GetError());

	m_Font = TTF_OpenFontRW(font_rw, 1, 24);

	if (m_Font == nullptr)
		ThrowRuntime("Failed to open font in SDL.", TTF_GetError());

	if (SDL_GetCurrentDisplayMode(0, &m_DisplayMode) < 0)
		ThrowRuntime("Failed to get current display mode.", SDL_GetError());
}

Window::~Window()
{
	TTF_CloseFont(m_Font);
	TTF_Quit();

	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Window::Run()
{
	SDL_Event event;

	Time time_prev = std::chrono::steady_clock::now();

	while (!m_Quit)
	{
		HandleEvents();
		UpdateInternals();
		RenderBackground();

		Time time_now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<nano>(time_now - time_prev);
		double dt = duration.count() / 1e9;

		if (dt > cfg::env::TIME_STEP)
		{
			if (m_Step)
			{
				m_Robot.Update(dt);
				m_Step = false;
				m_Pause = true;
			}
			else
			if (!m_Pause)
			{
				m_Robot.Update(dt);
			}
			time_prev = time_now;
		}

		RenderLinks();
		RenderJoints();
		//RenderInfo();

		SDL_RenderPresent(m_Renderer);
	}
}

void Window::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_s:
				m_Step = true;
				break;
			case SDLK_q:
				m_Quit = true;
				return;
			case SDLK_SPACE:
				m_Pause = !m_Pause;
				break;
			case SDLK_r:
				m_Robot.Restart();
				break;
			case SDLK_0:
				m_Robot.Restart(State{ 0.0, 0.0 });
				break;
			case SDLK_1:
				m_Robot.Restart(State{ 1.0, 0.0 });
				break;
			case SDLK_2:
				m_Robot.Restart(State{ 2.0, 0.0 });
				break;
			case SDLK_3:
				m_Robot.Restart(State{ 3.0, 0.0 });
				break;
			}
			break;
		case SDL_QUIT:
			m_Quit = true;
			return;
		}
	}
}

void Window::UpdateInternals()
{
	SDL_GetWindowSize(m_Window, &m_Width, &m_Height);
	m_CentreX = (double)m_Width / 2;
	m_CentreY = (double)m_Height / 2;
}

void Window::RenderBackground()
{
	SetColour(cfg::col::GREY);
	SDL_RenderClear(m_Renderer);
}

void Window::RenderJoints()
{
	auto DrawCircle = [this](const double x,
							 const double y,
							 const double r) -> void
		{
			int n_points = 0;
			std::array<SDL_Point, cfg::buf::MAX_OUTLINE_SIZE> outline{};

			const double inc = 1 / r;
			SetColour(cfg::joint::COLOUR);

			for (double a = 0.0; a < M_PI; a += inc)
			{
				const double rsina = r * sin(a);
				const int px = (int)(x + r * cos(a));
				const int py1 = (int)(y + rsina);
				const int py2 = (int)(y - rsina);
				outline[n_points++] = SDL_Point{ px, py1 };
				outline[n_points++] = SDL_Point{ px, py2 };
				SDL_RenderDrawLine(m_Renderer, px, py1, px, py2);
			}

			SetColour(cfg::col::BLACK);
			SDL_RenderDrawPoints(m_Renderer, outline.data(), n_points);
		};

	constexpr auto r1 = cfg::joint::RADIUS[0] * 1000.0;
	constexpr auto r2 = cfg::joint::RADIUS[1] * 1000.0;

	const Frame joints = m_Robot.GetJointFrames();
	const Coord coord1 = RobotToWindowFrame(joints[0]);
	const Coord coord2 = RobotToWindowFrame(joints[1]);

	DrawCircle(coord1.x, coord1.y, r1);
	DrawCircle(coord2.x, coord2.y, r2);
}

void Window::RenderLinks()
{
	auto DrawRectangle = [this](const double x,
								const double y,
								const double l,
								const double w,
								const double a) -> void
		{
			int n_points = 0;
			std::array<SDL_Point, cfg::buf::MAX_OUTLINE_SIZE> outline{};

			int pxl, pyl, pxr, pyr;
			const double inc = 1 / w;
			const double cosa = cos(a);
			const double sina = sin(a);
			const double w_cosa = w * cosa;
			const double w_sina = w * sina;
			const double nw_cosa = -w_cosa;
			const double nw_sina = -w_sina;
			const double l_2_cosa = l / 2 * cosa;
			const double l_2_sina = l / 2 * sina;
			const double l_2_cosa_px = l_2_cosa + x;
			const double l_2_sina_py = l_2_sina + y;
			const double nl_2_cosa_px = -l_2_cosa + x;
			const double nl_2_sina_py = -l_2_sina + y;
			SetColour(cfg::link::COLOUR);

			for (double t = -0.5; t < 0.5; t += inc)
			{
				pxl = (int)(l_2_cosa_px + nw_sina * t);
				pyl = (int)(l_2_sina_py + w_cosa * t);
				pxr = (int)(nl_2_cosa_px + nw_sina * t);
				pyr = (int)(nl_2_sina_py + w_cosa * t);
				outline[n_points++] = SDL_Point{ pxl, pyl };
				outline[n_points++] = SDL_Point{ pxr, pyr };
				SDL_RenderDrawLine(m_Renderer, pxl, pyl, pxr, pyr);
			}

			SetColour(cfg::col::BLACK);
			SDL_RenderDrawPoints(m_Renderer, outline.data(), n_points);
			const double w_2_cosa = w_cosa / 2;
			const double w_2_sina = w_sina / 2;
			const double nw_2_cosa = -w_2_cosa;
			const double nw_2_sina = -w_2_sina;
			pxl = (int)(l_2_cosa_px + nw_2_sina);
			pyl = (int)(l_2_sina_py + w_2_cosa);
			pxr = (int)(nl_2_cosa_px + nw_2_sina);
			pyr = (int)(nl_2_sina_py + w_2_cosa);
			SDL_RenderDrawLine(m_Renderer, pxl, pyl, pxr, pyr);
			pxl = (int)(l_2_cosa_px + w_2_sina);
			pyl = (int)(l_2_sina_py + nw_2_cosa);
			pxr = (int)(nl_2_cosa_px + w_2_sina);
			pyr = (int)(nl_2_sina_py + nw_2_cosa);
			SDL_RenderDrawLine(m_Renderer, pxl, pyl, pxr, pyr);
		};

	constexpr auto w1 = cfg::link::WIDTH[0] * 1000.0;
	constexpr auto w2 = cfg::link::WIDTH[1] * 1000.0;
	constexpr auto l1 = cfg::link::LENGTH[0] * 1000.0;
	constexpr auto l2 = cfg::link::LENGTH[1] * 1000.0;
	
	const Frame links = m_Robot.GetLinkFrames();
	const Coord coord1 = RobotToWindowFrame(links[0]);
	const Coord coord2 = RobotToWindowFrame(links[1]);

	const State& angles = m_Robot.GetPositions();
	const double a1 = -angles[0];
	const double a2 = -angles[0] - angles[1];

	DrawRectangle(coord1.x, coord1.y, l1, w1, a1);
	DrawRectangle(coord2.x, coord2.y, l2, w2, a2);
}

void Window::RenderInfo()
{
	SDL_Color colour = { 255, 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderText_Solid(m_Font, "hello", colour);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
	SDL_Rect rect = { 100, 100, surface->w, surface->h };
	SDL_FreeSurface(surface);
	SDL_RenderCopy(m_Renderer, texture, NULL, &rect);
}

void Window::SetColour(const int rgb[3])
{
	SDL_SetRenderDrawColor(m_Renderer, rgb[0], rgb[1], rgb[2], SDL_ALPHA_OPAQUE);
}

Coord Window::RobotToWindowFrame(const Coord& coord)
{
	return Coord
	{
		coord.x * 1000.0 + m_CentreX,
		m_CentreY - coord.y * 1000.0
	};
}
