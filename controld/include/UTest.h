#ifndef UTEST_H
#define UTEST_H

#include <stdio.h>

#include "Timer.h"

class UTest : public Timer
{
    public:
        UTest();
        virtual ~UTest();
        void wait(){Timer::wait();}

    protected:
        void AlarmFunction(){printf("alrm function\n");}
    private:
};

#endif // UTEST_H
