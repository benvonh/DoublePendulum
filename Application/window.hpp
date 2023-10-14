#pragma once

#include <chrono>
#include <memory>
#include <vector>
#include <stdexcept>

#include <math.h>
#include <SDL.h>

#include "Config.hpp"
#include "Robot.hpp"


using Time = std::chrono::steady_clock::time_point;

class Window
{
public:
	Window();
	~Window();

	void Run();

private:
	void RenderLinks();
	void RenderJoints();
	void RenderBackground();
	void InternalUpdate();
	void SetColour(const int rgb[3]);

	Coord RobotToWindowFrame(const Coord& coord);

	int m_Width;
	int m_Height;
	double m_CentreX;
	double m_CentreY;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_DisplayMode m_DisplayMode;
	Robot m_Robot;
};
