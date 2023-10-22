#pragma once

#include "Config.hpp"

#include <array>


struct Coord
{
	double x;
	double y;
};

using Frame = std::array<Coord, 2>;
using State = std::array<double, 2>;

class Robot
{
public:
	Robot();

	void Update(const double dt);
	void Restart();

	Frame GetLinkFrames();
	Frame GetJointFrames();
	const State& GetPositions();
	const State& GetVelocities();
	const State& GetAccelerations();

private:
	State m_Pos;
	State m_Vel;
	State m_Acc;
};