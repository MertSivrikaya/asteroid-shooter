#ifndef _TIMER_
#define _TIMER_

class Timer
{
    public:
        Timer();
        Timer(double duration);

        void start();               // Start the timer
        void update();              // Update the timer
        void reset();               // Reset the timer

        bool active;                // A variable to determine whether the timer is active

    private:
        double startTime;           
        double duration;

};

#endif
