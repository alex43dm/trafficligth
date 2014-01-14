#ifndef TIMER_H
#define TIMER_H

#include <signal.h>
#include <time.h>

class Timer
{
public:
    Timer();
    virtual ~Timer();
    void start();
    void wait();
protected:
    virtual void AlarmFunction() = 0;
private:
    timer_t timerID;
    struct itimerspec timerSpecs;

    static void alarmFunction(int sigNumb, siginfo_t *si, void *uc);

};

#endif // TIMER_H
