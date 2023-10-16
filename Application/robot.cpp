#include "robot.hpp"


Robot::Robot() : m_Pos{}, m_Vel{}, m_Acc{}
{
}

void Robot::Update(const double dt)
{
	double a1, a2;
	double trq1, trq2;

	// Rename important variables
	const double q1 = m_Pos[0];
	const double q2 = m_Pos[1];
	const double w1 = m_Vel[0];
	const double w2 = m_Vel[1];
	constexpr double g = cfg::env::GRAVITY;
	constexpr double m1 = cfg::link::MASS[0];
	constexpr double m2 = cfg::link::MASS[1];
	constexpr double L1 = cfg::link::LENGTH[0];
	constexpr double L2 = cfg::link::LENGTH[1];
	constexpr double u1 = cfg::joint::FRICTION[0];
	constexpr double u2 = cfg::joint::FRICTION[1];

	// Set input torque
	trq1 = -u1 * w1;
	trq2 = -u2 * w2;

	// Compute forward dynamics
	const double cosq1 = cos(q1);
	const double cosq2 = cos(q2);
	const double sinq1 = sin(q1);
	const double sinq2 = sin(q2);
	const double cosq12 = cos(q1 + q2);
	a1 = (trq1 - L1 * g * cosq1 * (m1 + m2) - L2 * g * m2 * cosq12 + L1 * L2 * m2 * w2 * sinq2 * (2 * w1 + w2)) / (L1 * L1 * (-m2 * cosq2 * cosq2 + m1 + m2)) + ((L2 + L1 * cosq2) * (L1 * L2 * m2 * sinq2 * w1 * w1 - trq2 + L2 * g * m2 * cosq12)) / (L1 * L1 * L2 * (-m2 * cosq2 * cosq2 + m1 + m2));
	a2 = -((L2 + L1 * cosq2) * (trq1 - L1 * g * cosq1 * (m1 + m2) - L2 * g * m2 * cosq12 + L1 * L2 * m2 * w2 * sinq2 * (2 * w1 + w2))) / (L1 * L1 * L2 * (-m2 * cosq2 * cosq2 + m1 + m2)) - ((L1 * L2 * m2 * sinq2 * w1 * w1 - trq2 + L2 * g * m2 * cosq12) * (L1 * L1 * m1 + L1 * L1 * m2 + L2 * L2 * m2 + 2 * L1 * L2 * m2 * cosq2)) / (L1 * L1 * L2 * L2 * m2 * (-m2 * cosq2 * cosq2 + m1 + m2));

	// Update joint states
	m_Pos[0] = q1 + w1 * dt;
	m_Pos[1] = q2 + w2 * dt;
	m_Vel[0] = w1 + a1 * dt;
	m_Vel[1] = w2 + a2 * dt;
	m_Acc[0] = a1;
	m_Acc[1] = a2;
}

void Robot::Restart(const State& s)
{
	m_Pos = s;
	m_Vel = { 0.0, 0.0 };
	m_Acc = { 0.0, 0.0 };
}

Frame Robot::GetLinkFrames()
{
	const double cosq1 = cos(m_Pos[0]);
	const double sinq1 = sin(m_Pos[0]);
	const double cosq12 = cos(m_Pos[0] + m_Pos[1]);
	const double sinq12 = sin(m_Pos[0] + m_Pos[1]);
	constexpr double L1 = cfg::link::LENGTH[0];
	constexpr double L1_2 = cfg::link::LENGTH[0] / 2;
	constexpr double L2_2 = cfg::link::LENGTH[1] / 2;

	return Frame
	{
		Coord
		{
			L1_2 * cosq1,
			L1_2 * sinq1
		},
		Coord
		{
			L1 * cosq1 + L2_2 * cosq12,
			L1 * sinq1 + L2_2 * sinq12
		}
	};
}

Frame Robot::GetJointFrames()
{
	const double q1 = m_Pos[0];
	constexpr double L1 = cfg::link::LENGTH[0];

	return Frame
	{
		Coord{ 0.0, 0.0 },
		Coord{ L1 * cos(q1), L1 * sin(q1) }
	};
}

const State& Robot::GetPositions()
{
	return m_Pos;
}

const State& Robot::GetVelocities()
{
	return m_Vel;
}

const State& Robot::GetAccelerations()
{
	return m_Acc;
}
