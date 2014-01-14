#ifndef PROCESS_H
#define PROCESS_H

#include <queue>

#include "Config.h"
#include "Timer.h"
#include "Cron.h"
#include "Proto.h"
#include "Serial.h"
#include "TcpSocketServer.h"
#include "ShMem.h"

#define MAX_CMD_QUEUE_LEN 10

class Process : public Timer//, public TcpSocketServer
{
    public:
        int *ledsVal, *WAY1, *WAY2, *WAY3, *WAY4;
        int ledsLen;

        Process();
        virtual ~Process();
        int run();
        void setPhase(int _phase);
        void WaitSec();
        void setNextPhase();
        bool isTVPpressed(){ return tvp.size() > 0 ? true : false; }
        int getProgrmId(int);
    protected:
        void AlarmFunction();
        bool cmd();
        void setTvpVal();
    private:
        Cron *cr;
        Serial *mega, *leds;
        Proto *bufCmdMega, *bufCmdLeds;
        ProgramPhase *PHASE1, *PHASE2, *PHASE3;

        unsigned short dio, dio_last;
        std::queue<unsigned short> tvp;
        unsigned short curTvp;

        bool getControll;
        int phase;
        ShMem *shm;
        int countDown;
        bool tvpActive, isK1orK2, isK3orK4;
        int curProgramId, lastProgramId;
};

#endif // PROCESS_H
