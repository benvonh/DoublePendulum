#pragma once

#include "Font.hpp"
#include "Robot.hpp"
#include "Config.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <math.h>

#include <array>
#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>


using nano = std::chrono::nanoseconds;
using Time = std::chrono::steady_clock::time_point;

class Window
{
public:
	Window();
	~Window();

	void Run();

private:
	void UpdateInternals();
	void UpdateRobot();
	void RenderBackground();
	void RenderLinks();
	void RenderJoints();
	void RenderInfo();
	void HandleEvents();
	void SetColour(const int rgb[3]);
	Coord RobotToWindowFrame(const Coord& coord);

	int m_Width;
	int m_Height;
	double m_CentreX;
	double m_CentreY;
	double m_DeltaTime;
	double m_DeltaTimeSim;
	double m_DeltaTimeInfo;
	Time m_Time;
	Time m_TimeSim;
	Time m_TimeInfo;
	bool m_StepSim;
	bool m_StepInfo;
	bool m_OneStep;
	bool m_Quit;
	bool m_Pause;
	Robot m_Robot;

	TTF_Font* m_Font;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_Texture* m_Texture;
	SDL_Texture* m_TextureSim;
	SDL_Texture* m_TextureInfo;
	SDL_Rect m_TextureArea;
	SDL_Rect m_TextureAreaSim;
	SDL_Rect m_TextureAreaInfo;
};
