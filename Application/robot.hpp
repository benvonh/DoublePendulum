#pragma once

#include <array>

#include "Config.hpp"


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

	void Update(double dt);
	void Restart(const State& s=State{ 0.0, 0.0 });

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