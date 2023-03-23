#pragma once
#include "MouseEvent.h"
#include <queue>

class Mouse
{
public:
	void onLeftPressed(int x, int y);
	void onLeftReleased(int x, int y);
	void onRightPressed(int x, int y);
	void onRightReleased(int x, int y);
	void onMiddlePressed(int x, int y);
	void onMiddleReleased(int x, int y);
	void onWheelUp(int x, int y);
	void onWheelDown(int x, int y);
	void onMouseMove(int x, int y);
	void onMouseMoveRaw(int x, int y);

	bool isLeftDown() const { return m_leftDown; };
	bool isRightDown() const { return m_rightDown; };
	bool isMiddleDown() const { return m_middleDown; };

	int getX() const { return m_x; };
	int getY() const { return m_y; };
	MousePoint getPos() const { return MousePoint{ m_x, m_y }; };

	bool isEventBufferEmpty() const { return m_eventBuffer.empty(); };
	MouseEvent readEvent();

private:
	std::queue<MouseEvent> m_eventBuffer;
	bool m_leftDown = false;
	bool m_rightDown = false;
	bool m_middleDown = false;
	int m_x = 0;
	int m_y = 0;
};