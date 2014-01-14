#ifndef CRONPROGRAM_H
#define CRONPROGRAM_H

#include <time.h>

class CronProgram
{
    public:
        int Id;
        struct tm StarTime;
        int Offset;

        CronProgram();
        virtual ~CronProgram();
        bool operator() (const CronProgram &x, const CronProgram &y)
        {
            return x.StarTime.tm_hour >= y.StarTime.tm_hour && x.StarTime.tm_min >= y.StarTime.tm_min;
        }
        bool DoesItStartNow(struct tm*);
    protected:
    private:
};

#endif // CRONPROGRAM_H
