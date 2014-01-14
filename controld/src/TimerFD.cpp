#include <strings.h>
#include <unistd.h>

#include "TimerFD.h"
#include "Config.h"

TimerFD::TimerFD()
{
    bzero(&its, sizeof(struct itimerspec));
}

TimerFD::~TimerFD()
{
    //dtor
}

int TimerFD::start()
{
    fdTimer = timerfd_create (CLOCK_MONOTONIC, 0);

    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = Config::Instance()->timer_val * 1000 * 1000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    timerfd_settime (fdTimer, 0, &its, NULL);
    return 0;
}

int TimerFD::wait()
{
    unsigned long long msd;

    return ::read(fdTimer, &msd, sizeof(msd));
}
