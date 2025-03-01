#include "timer.h"

#include "raylib.h"
#include <iostream>

Timer::Timer() : duration(0.0), active(false), startTime(0.0) {};
Timer::Timer(double duration) : duration(duration), active(false), startTime(0.0) {};

void Timer::start()
{
    active = true;
    startTime = GetTime();
}

void Timer::update()
{
    if (active)
    {
        double currentTime = GetTime();
        // std::cout << "Timer running: " << (currentTime - startTime) << " / " << duration << " seconds" << std::endl;
        
        if ((currentTime - startTime) > duration)
        {
            reset();
        }
    }
}

void Timer::reset()
{
    active = false;
}
