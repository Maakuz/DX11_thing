#pragma once
#include "KeyboardEvent.h"
#include <queue>

class Keyboard
{
public:
	Keyboard() {};
	bool isKeyPressed(const unsigned char keycode);
	bool isKeyBufferEmpty();
	bool isCharBufferEmpty();
	bool isKeysAutoRepeat() { return m_autoRepeatKeys; }
	bool isCharsAutoRepeat() { return m_autoRepeatChars; }

	KeyboardEvent readKey();
	unsigned char readChar();
	void onKeyPressed(const unsigned char key);
	void onKeyReleased(const unsigned char key);
	void onChar(const unsigned char key);

	void enableAutoRepeatKeys() { m_autoRepeatKeys = true; }
	void disableAutoRepeatKeys() { m_autoRepeatKeys = false; }
	void enableAutoRepeatChars() { m_autoRepeatChars = true; }
	void disableAutoRepeatChars() { m_autoRepeatChars = false; }

private:
	bool m_autoRepeatKeys = false;
	bool m_autoRepeatChars = false;
	bool m_keyStates[256] = {false};
	std::queue<KeyboardEvent> m_keyBuffer;
	std::queue<unsigned char> m_charBuffer;
	
};