#pragma once
class KeyboardEvent
{
public:
	enum EventType
	{
		release = 0,
		press = 1,
		invalid = 2
	};

	KeyboardEvent() :
		m_type(EventType::invalid),
		m_key(0u) {}

	KeyboardEvent(const EventType type, const unsigned char key): 
		m_type(type),
		m_key(key) {}
	
	bool isPress() const { return m_type == EventType::press; };
	bool isRelease() const { return m_type == EventType::release; };
	bool isValid() const { return m_type != EventType::invalid; };
	unsigned char getKeyCode() const { return m_key; };

private:
	EventType m_type;
	unsigned char m_key;
};

