#include "Keyboard.h"

bool Keyboard::isKeyPressed(const unsigned char keycode)
{
    return m_keyStates[keycode];
}

bool Keyboard::isKeyBufferEmpty()
{
    return m_keyBuffer.empty();
}

bool Keyboard::isCharBufferEmpty()
{
    return m_charBuffer.empty();
}

KeyboardEvent Keyboard::readKey()
{
    if (m_keyBuffer.empty())
    {
        return KeyboardEvent();
    }
    
    else
    {
        KeyboardEvent e = m_keyBuffer.front();
        m_keyBuffer.pop();
        return e;
    }
}

unsigned char Keyboard::readChar()
{
    if (m_charBuffer.empty())
    {
        return 0u;
    }

    else
    {
        unsigned char e = m_charBuffer.front();
        m_charBuffer.pop();
        return e;
    }
}

void Keyboard::onKeyPressed(const unsigned char key)
{
    m_keyStates[key] = true;
    m_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::press, key));
}

void Keyboard::onKeyReleased(const unsigned char key)
{
    m_keyStates[key] = false;
    m_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::release, key));
}

void Keyboard::onChar(const unsigned char key)
{
    m_charBuffer.push(key);
}