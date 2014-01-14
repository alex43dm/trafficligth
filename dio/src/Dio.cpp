#include <iostream>
#include <sstream>
#include <ctime>

#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <strings.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "Dio.h"
#include "Log.h"
#include "ShMem.h"
#include "Config.h"

Dio::Dio()
{
}

Dio::~Dio()
{
}

int Dio::loop()
{
    ShMem *shm = new ShMem(true);
    spi = new Spi(spiDev, spiMode, spiSpeed, spiBitsPerWord);

    int sleepTime = 1000000 * atof(period.c_str());

    Log::info("period %d mks", sleepTime);

    unsigned long long msd;
    struct itimerspec its;
    bzero(&its, sizeof(struct itimerspec));
    int fdTimer = timerfd_create (CLOCK_MONOTONIC, 0);

    if(sleepTime > 1000000)
    {
        its.it_value.tv_sec = sleepTime / 1000000;
        its.it_value.tv_nsec = (sleepTime % 1000000)*1000;
    }
    else
    {
        its.it_value.tv_sec = 0;
        its.it_value.tv_nsec = sleepTime * 1000;
    }
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    timerfd_settime (fdTimer, 0, &its, NULL);

    Log::info("dio main loop start");
    for(;;)
    {
        unsigned short ret = spi->GetGPIO();
        //write to shared memory
        shm->write(ret);

        if(::read(fdTimer, &msd, sizeof(msd)) == 0)
        {
            Log::err("error read timer fd");
        }
    }
    return 0;
}

