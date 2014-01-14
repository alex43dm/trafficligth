#include <strings.h>

#include "CronProgram.h"

CronProgram::CronProgram()
{
    //ctor
    bzero(&StarTime,sizeof(StarTime));
}

CronProgram::~CronProgram()
{
    //dtor
}

bool CronProgram::DoesItStartNow(struct tm *now)
{
    if(now->tm_hour > StarTime.tm_hour || ( now->tm_hour == StarTime.tm_hour && now->tm_min >= StarTime.tm_min))
    {
        return true;
    }

    return false;
}
