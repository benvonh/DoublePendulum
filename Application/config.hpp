#pragma once


// Application configuration
namespace cfg
{
	// Window settings
	namespace win
	{
		constexpr int DEFAULT_WIDTH = 1280;
		constexpr int DEFAULT_HEIGHT = 720;
	}

	// Colour palette
	namespace col
	{
		constexpr int RED[3]	= { 251,  73,  52 };
		constexpr int GREEN[3]	= { 184, 187,  38 };
		constexpr int YELLOW[3] = { 250, 189,  47 };
		constexpr int BLUE[3]	= { 131, 165, 152 };
		constexpr int PURPLE[3] = { 211, 134, 155 };
		constexpr int AQUA[3]	= { 142, 192, 124 };
		constexpr int GREY[3]	= { 146, 131, 116 };
		constexpr int BLACK[3]	= {  40,  40,  40 };
	}

	// Link parameters
	namespace link
	{
		constexpr auto COLOUR = col::YELLOW;
		constexpr double MASS[2] = { 1.0, 1.0 };
		constexpr double WIDTH[2] = { 0.01, 0.01 };
		constexpr double LENGTH[2] = { 0.2, 0.2 };
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
		constexpr double TIME_STEP = 1e-6;
	}

	// Buffer settings
	namespace buf
	{
		constexpr int MAX_OUTLINE_SIZE = 1024;
	}
}
