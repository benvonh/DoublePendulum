#pragma once

#include <array>
#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

#include <math.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Config.hpp"
#include "Robot.hpp"
#include "Font.hpp"


using nano = std::chrono::nanoseconds;
using Time = std::chrono::steady_clock::time_point;

class Window
{
public:
	Window();
	~Window();

	void Run();

private:
	void HandleEvents();
	void UpdateInternals();
	void RenderBackground();
	void RenderJoints();
	void RenderLinks();
	void RenderInfo();
	void SetColour(const int rgb[3]);
	Coord RobotToWindowFrame(const Coord& coord);

	int m_Width;
	int m_Height;
	double m_CentreX;
	double m_CentreY;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_DisplayMode m_DisplayMode;
	TTF_Font* m_Font;

	bool m_Step;
	bool m_Quit;
	bool m_Pause;
	Robot m_Robot;
};
