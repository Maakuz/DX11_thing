#pragma once
#include <chrono>

class Timer 
{
public:
    Timer();
    double millisecondsElapsed();
    void restart();
    bool start();
    bool stop();
private:
    bool m_running = false;

#ifdef _WIN32
    std::chrono::time_point<std::chrono::steady_clock> m_start;
    std::chrono::time_point<std::chrono::steady_clock> m_stop;
#else
    std::chrono::time_point<std::chrono::system_clock> m_start;
    std::chrono::time_point<std::chrono::system_clock> m_stop;
#endif // _win32
};