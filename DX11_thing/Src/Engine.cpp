#include "Engine.h"
using namespace DirectX;

bool Engine::initialize(HINSTANCE hInstance, std::string title, std::string windowClass, int width, int height)
{
    if (!m_renderWindow.initialize(this, hInstance, title, windowClass, width, height))
        return false;

    if (!m_gfx.initialize(m_renderWindow.getHWND(), width, height))
        return false;

    return true;
}

bool Engine::processMessage()
{
    return m_renderWindow.processMessage();
}

bool Engine::update()
{
    while (!m_keyboard.isCharBufferEmpty())
    {
        unsigned char l_char = m_keyboard.readChar();

    }

    while (!m_keyboard.isKeyBufferEmpty())
    {
        KeyboardEvent l_event = m_keyboard.readKey();
        std::string str;
        str += std::to_string(l_event.getKeyCode());
        OutputDebugStringA(str.c_str());
    }

    const float CAMERA_SPEED = 0.01f;
    const float CAMERA_SENSITIVITY = 0.001f;

    while (!m_mouse.isEventBufferEmpty())
    {
        MouseEvent l_event = m_mouse.readEvent();

        if (l_event.getType() == MouseEvent::RawMove)
        {
            m_gfx.m_camera.rotate(l_event.getY() * CAMERA_SENSITIVITY, l_event.getX() * CAMERA_SENSITIVITY, 0.f);
        }
    }

    if (m_keyboard.isKeyPressed('W'))
    {
        m_gfx.m_camera.move(m_gfx.m_camera.getForward() * CAMERA_SPEED);
    }

    if (m_keyboard.isKeyPressed('S'))
    {
        m_gfx.m_camera.move(-m_gfx.m_camera.getForward() * CAMERA_SPEED);
    }

    if (m_keyboard.isKeyPressed('A'))
    {
        m_gfx.m_camera.move(-m_gfx.m_camera.getRight() * CAMERA_SPEED);
    }

    if (m_keyboard.isKeyPressed('D'))
    {
        m_gfx.m_camera.move(m_gfx.m_camera.getRight() * CAMERA_SPEED);
    }

    if (m_keyboard.isKeyPressed(32)) //space
    {
        m_gfx.m_camera.move(0, CAMERA_SPEED, 0);
    }

    if (m_keyboard.isKeyPressed(17)) //CTRL
    {
        m_gfx.m_camera.move(0, -CAMERA_SPEED, 0);
    }



    if (m_keyboard.isKeyPressed(27)) //ESC - Close application
    {
        return false;
    }

    return true;
}

void Engine::render()
{
    m_gfx.render();
}
