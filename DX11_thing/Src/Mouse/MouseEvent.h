#pragma once
struct MousePoint
{
	int x;
	int y;
};

class MouseEvent
{
public:
	enum EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		WheelPress,
		WheelRelease,
		WheelUp,
		WheelDown,
		Move,
		RawMove,
		Invalid
	};
	
	MouseEvent(): 
		m_type(EventType::Invalid),
		m_x(0),
		m_y(0) {}

	MouseEvent(const EventType type, const int x, const int y):
		m_type(type),
		m_x(x),
		m_y(y) {}

	bool isValid() const { return m_type != EventType::Invalid; };
	EventType getType() const { return m_type; };
	MousePoint getPos() const { return MousePoint{ m_x, m_y }; };
	int getX() const { return m_x; }
	int getY() const { return m_y; }

private:
	EventType m_type;
	int m_x;
	int m_y;
};