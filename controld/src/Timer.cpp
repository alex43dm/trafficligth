#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "Timer.h"

Timer::Timer()
{
    timerSpecs.it_value.tv_sec = 0;
    timerSpecs.it_value.tv_nsec = 200 * 1000 * 1000;
    timerSpecs.it_interval.tv_sec = timerSpecs.it_value.tv_sec;
    timerSpecs.it_interval.tv_nsec = timerSpecs.it_value.tv_nsec;

    struct sigaction SignalAction;
    sigemptyset(&SignalAction.sa_mask);
    SignalAction.sa_flags = SA_SIGINFO;
    SignalAction.sa_sigaction = Timer::alarmFunction;

    if (sigaction(SIGRTMIN, &SignalAction, NULL))
    {
        printf("Could not install new signal handler\n");
        exit(1);
    }

    struct sigevent signalEvent;
    memset(&signalEvent, 0, sizeof(signalEvent));
    signalEvent.sigev_notify = SIGEV_SIGNAL;
    signalEvent.sigev_value.sival_ptr = (void*) this;
    signalEvent.sigev_signo = SIGRTMIN;

    if (timer_create(CLOCK_REALTIME, &signalEvent, &timerID) != 0)
    {
        printf("Could not creat the timer\n");
        exit(1);
    }
}

Timer::~Timer()
{
    //dtor
}


void Timer::start()
{
    if (timer_settime(timerID, 0, &timerSpecs, NULL) == -1)
    {
        printf("Could not start timer %s\n", strerror(errno));
        exit(1);
    }
}

void Timer::alarmFunction(int sigNumb, siginfo_t *si, void *uc)
{
    Timer * ptrTimerClass = reinterpret_cast<Timer *> (si->si_value.sival_ptr);
    ptrTimerClass->AlarmFunction();
}

void Timer::wait()
{
  pause();
}
