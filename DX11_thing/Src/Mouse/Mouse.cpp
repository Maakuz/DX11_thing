#include "Mouse.h"

void Mouse::onLeftPressed(int x, int y)
{
	m_leftDown = true;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void Mouse::onLeftReleased(int x, int y)
{
	m_leftDown = false;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void Mouse::onRightPressed(int x, int y)
{
	m_rightDown = true;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void Mouse::onRightReleased(int x, int y)
{
	m_rightDown = false;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void Mouse::onMiddlePressed(int x, int y)
{
	m_middleDown = true;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelPress, x, y));
}

void Mouse::onMiddleReleased(int x, int y)
{
	m_middleDown = false;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelRelease, x, y));
}

void Mouse::onWheelUp(int x, int y)
{
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void Mouse::onWheelDown(int x, int y)
{
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void Mouse::onMouseMove(int x, int y)
{
	m_x = x;
	m_y = y;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void Mouse::onMouseMoveRaw(int x, int y)
{
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::RawMove, x, y));
}

MouseEvent Mouse::readEvent()
{
	if (m_eventBuffer.empty())
	{
		return MouseEvent();
	}

	else
	{
		MouseEvent l_event = m_eventBuffer.front();
		m_eventBuffer.pop();
		return l_event;
	}
}
