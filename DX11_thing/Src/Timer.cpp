#include "Timer.h"

Timer::Timer()
{
    m_start = std::chrono::high_resolution_clock::now();
    m_stop = std::chrono::high_resolution_clock::now();
}

double Timer::millisecondsElapsed()
{
    if (m_running)
    {
        auto l_elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_start);
        return l_elapsed.count();
    }

    else
    {
        auto l_elapsed = std::chrono::duration<double, std::milli>(m_stop - m_start);
        return l_elapsed.count();
    }
}

void Timer::restart()
{
    m_running = true;
    m_start = std::chrono::high_resolution_clock::now();
}

bool Timer::start()
{
    if (m_running)
        return false;

    else
    {
        m_start = std::chrono::high_resolution_clock::now();
        m_running = true;

        return true;
    }
}

bool Timer::stop()
{
    if (!m_running)
        return false;

    else
    {
        m_stop = std::chrono::high_resolution_clock::now();
        m_running = false;

        return true;
    }
}
