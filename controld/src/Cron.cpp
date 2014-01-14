#include <stdio.h>

#include "Cron.h"
#include "Log.h"
#include "Config.h"

Cron::Cron()
{

}

Cron::~Cron()
{
    //delete cDays;
}

int Cron::getProgram(int toTime)
{
    time_t now;
    struct tm tmnow;

    time(&now);

    now += toTime;

    tmnow = *localtime(&now);

    CronDay *cd = (*Config::Instance()->cDays)[tmnow.tm_wday];
    int pId = cd->GetProgamId(&tmnow);

    if(pId == -1)
    {
        if(tmnow.tm_wday != 0)
            pId = (*Config::Instance()->cDays)[tmnow.tm_wday-1]->GetLastProgramId();
        else
            pId = (*Config::Instance()->cDays)[6]->GetLastProgramId();
            pId = (*Config::Instance()->cDays)[6]->GetLastProgramId();
    }
    pId--;
    Log::info("set prog: %d", pId);
    return pId;
}
