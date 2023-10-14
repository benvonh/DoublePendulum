#include "window.hpp"


Window::Window() : m_Width{}, m_Height{}, m_CentreX{}, m_CentreY{}, m_Robot{}
{
	SDL_Init(SDL_INIT_VIDEO);

	m_Window = SDL_CreateWindow(
		"Double Pendulum",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		cfg::win::DEFAULT_WIDTH,
		cfg::win::DEFAULT_HEIGHT,
		SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
	);

	m_Renderer = SDL_CreateRenderer(
		m_Window, -1,
		SDL_RENDERER_ACCELERATED
	);

	SDL_GetCurrentDisplayMode(0, &m_DisplayMode);
}

Window::~Window()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Window::Run()
{
	SDL_Event event;

	bool step = false;
	bool pause = false;
	Time time_prev = std::chrono::steady_clock::now();

	while (true)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
					pause = !pause;
					break;
				case SDLK_s:
					step = true;
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
				return;
			}
		}
		InternalUpdate();
		RenderBackground();

		Time time_now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(time_now - time_prev);
		double dt = duration.count() / 1e9;

		if (dt > cfg::env::TIME_STEP)
		{
			if (step)
			{
				m_Robot.Update(dt);
				step = false;
				pause = true;
			}
			else if (!pause)
			{
				m_Robot.Update(dt);
			}
			time_prev = time_now;
		}
		RenderLinks();
		RenderJoints();

		SDL_RenderPresent(m_Renderer);
	}
}

void Window::RenderLinks()
{
	auto DrawRectangle = [this](const double x,
								const double y,
								const double l,
								const double w,
								const double a) -> void
		{
			SDL_Point outline[cfg::buf::MAX_OUTLINE_SIZE] = {};
			size_t n_points = 0;

			const double inc = 1 / w;
			const double l_2 = l / 2;
			const double cosa = cos(a);
			const double sina = sin(a);
			SetColour(cfg::link::COLOUR);

			for (double t = -0.5; t < 0.5; t += inc)
			{
				const int pxl = (int)(l_2 * cosa - w * t * sina + x);
				const int pyl = (int)(l_2 * sina + w * t * cosa + y);
				const int pxr = (int)(-l_2 * cosa - w * t * sina + x);
				const int pyr = (int)(-l_2 * sina + w * t * cosa + y);
				outline[n_points++] = SDL_Point{ pxl, pyl };
				outline[n_points++] = SDL_Point{ pxr, pyr };
				SDL_RenderDrawLine(m_Renderer, pxl, pyl, pxr, pyr);
			}
			SetColour(cfg::col::BLACK);
			SDL_RenderDrawPoints(m_Renderer, (SDL_Point*)outline, (int)n_points);
			int pxl = (int)(l_2 * cosa - w * -0.5 * sina + x);
			int pyl = (int)(l_2 * sina + w * -0.5 * cosa + y);
			int pxr = (int)(-l_2 * cosa - w * -0.5 * sina + x);
			int pyr = (int)(-l_2 * sina + w * -0.5 * cosa + y);
			SDL_RenderDrawLine(m_Renderer, pxl, pyl, pxr, pyr);
			pxl = (int)(l_2 * cosa - w * 0.5 * sina + x);
			pyl = (int)(l_2 * sina + w * 0.5 * cosa + y);
			pxr = (int)(-l_2 * cosa - w * 0.5 * sina + x);
			pyr = (int)(-l_2 * sina + w * 0.5 * cosa + y);
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

void Window::RenderJoints()
{
	auto DrawCircle = [this](const double x,
							 const double y,
							 const double r) -> void
		{
			SDL_Point outline[cfg::buf::MAX_OUTLINE_SIZE] = {};
			size_t n_points = 0;

			const double inc = 1 / r;
			SetColour(cfg::joint::COLOUR);

			for (double a = 0.0; a < M_PI; a += inc)
			{
				const double cache = r * sin(a);
				const int px = (int)(x + r * cos(a));
				const int py1 = (int)(y + cache);
				const int py2 = (int)(y - cache);
				outline[n_points++] = SDL_Point{ px, py1 };
				outline[n_points++] = SDL_Point{ px, py2 };
				SDL_RenderDrawLine(m_Renderer, px, py1, px, py2);
			}
			SetColour(cfg::col::BLACK);
			SDL_RenderDrawPoints(m_Renderer, (SDL_Point*)outline, (int)n_points);
		};

	constexpr auto r1 = cfg::joint::RADIUS[0] * 1000.0;
	constexpr auto r2 = cfg::joint::RADIUS[1] * 1000.0;

	const Frame joints = m_Robot.GetJointFrames();
	const Coord coord1 = RobotToWindowFrame(joints[0]);
	const Coord coord2 = RobotToWindowFrame(joints[1]);

	DrawCircle(coord1.x, coord1.y, r1);
	DrawCircle(coord2.x, coord2.y, r2);
}

void Window::RenderBackground()
{
	SetColour(cfg::col::GREY);
	SDL_RenderClear(m_Renderer);
}

void Window::InternalUpdate()
{
	SDL_GetWindowSize(m_Window, &m_Width, &m_Height);
	m_CentreX = (double)m_Width / 2;
	m_CentreY = (double)m_Height / 2;
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
