#include "Engine.h"

bool Engine::initialize(HINSTANCE hInstance, std::string title, std::string windowClass, int width, int height)
{
    return m_renderWindow.initialize(this, hInstance, title, windowClass, width, height);
}

bool Engine::processMessage()
{
    return m_renderWindow.processMessage();
}

void Engine::update()
{
    while (!m_keyboard.isCharBufferEmpty())
    {
        unsigned char l_char = m_keyboard.readChar();

    }

    while (!m_keyboard.isKeyBufferEmpty())
    {
        KeyboardEvent l_event = m_keyboard.readKey();

    }

    while (!m_mouse.isEventBufferEmpty())
    {
        MouseEvent l_event = m_mouse.readEvent();

    }
}
