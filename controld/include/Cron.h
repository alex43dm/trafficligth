#ifndef CRON_H
#define CRON_H

#include <array>
#include "CronDay.h"
#include "Program.h"

class Cron
{
    public:
        Cron();
        virtual ~Cron();
        static int getProgramId(int toTime = 0){ return getProgram(toTime); }
        static int getNextProgramId(){ return getProgram(1); }
    protected:
    private:
        static int getProgram(int);
};

#endif // CRON_H
