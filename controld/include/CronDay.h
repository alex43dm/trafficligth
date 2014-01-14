#ifndef CRONDAY_H
#define CRONDAY_H

#include "CronProgram.h"
#include <vector>

class CronDay
{
    public:
        std::vector<CronProgram*> *cPrograms;
        int Id;

        CronDay();
        virtual ~CronDay();

        int GetProgamId(struct tm *);
        int GetLastProgramId();
        void Sort();
    protected:
    private:

    std::vector<CronProgram*>::iterator curProgram;

    CronProgram *GetProramByTime(struct tm *);
};

#endif // CRONDAY_H
