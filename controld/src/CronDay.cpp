#include <algorithm>

#include "CronDay.h"

CronDay::CronDay()
{
    //to begin
    //curProgram = cPrograms->begin();

    cPrograms = new std::vector<CronProgram*>();
}

CronDay::~CronDay()
{
    delete cPrograms;
}

CronProgram *CronDay::GetProramByTime(struct tm *now)
{
    for (std::vector<CronProgram*>::iterator it = cPrograms->begin() ; it != cPrograms->end(); ++it)
    {
        CronProgram *c = (CronProgram *)*it;
        if(c->DoesItStartNow(now))
        {
            curProgram = it;
            return c;
        }
    }

    return nullptr;
}

int CronDay::GetProgamId(struct tm *now)
{
    CronProgram *c = NULL;

    for (auto it = cPrograms->rbegin(); it != cPrograms->rend(); ++it)
    {
        if((*it)->DoesItStartNow(now))
        {
            c = *it;
            return c->Id;
        }
    }

    return -1;
}

int CronDay::GetLastProgramId()
{
    return (cPrograms->back())->Id;
}

void CronDay::Sort()
{
    std::sort(cPrograms->begin(), cPrograms->end());
}
