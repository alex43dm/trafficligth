#ifndef TIMERFD_H
#define TIMERFD_H

#include <sys/timerfd.h>

class TimerFD
{
    public:
        TimerFD();
        virtual ~TimerFD();
        int start();
        virtual int wait();
    protected:
    private:
        struct itimerspec its;
        int fdTimer;
};

#endif // TIMERFD_H
