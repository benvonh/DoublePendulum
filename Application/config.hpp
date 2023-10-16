#pragma once


// Application configuration
namespace cfg
{
	// Window settings
	namespace win
	{
		constexpr int FONT_SIZE = 18;
		constexpr int DEFAULT_WIDTH = 1280;
		constexpr int DEFAULT_HEIGHT = 720;
	}

	// Colour palette
	namespace col
	{
		constexpr int RED[3]     = { 204,   0,   0 };
		constexpr int GREEN[3]   = {  78, 154,   6 };
		constexpr int YELLOW[3]  = { 196, 160,   0 };
		constexpr int BLUE[3]    = {  52, 101, 164 };
		constexpr int PURPLE[3]  = { 108, 113, 196 };
		constexpr int AQUA[3]    = {   6, 152, 154 };
		constexpr int GREY[3]    = {  28,  28,  28 };
		constexpr int BLACK[3]   = {  40,  40,  40 };
		constexpr int WHITE[3]   = { 235, 219, 178 };
	}

	// Link parameters
	namespace link
	{
		constexpr auto COLOUR = col::YELLOW;
		constexpr double MASS[2] = { 1.0, 1.0 };
		constexpr double WIDTH[2] = { 0.01, 0.01 };
		constexpr double LENGTH[2] = { 0.1, 0.1 };
	}

	// Joint parameters
	namespace joint
	{
		constexpr auto COLOUR = col::RED;
		constexpr double RADIUS[2] = { 0.01, 0.01 };
		constexpr double FRICTION[2] = {0.001, 0.001};
	}

	// Physics environment
	namespace env
	{
		constexpr double GRAVITY = 9.81;
		constexpr double SIM_TIME = 1e-9;
		constexpr double INFO_TIME = 1e-1;
	}

	// Buffer settings
	namespace buf
	{
		constexpr size_t MAX_OUTLINE_SIZE = 1024;
	}
}
