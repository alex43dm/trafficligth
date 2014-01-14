#include <unistd.h>

#include "Log.h"
#include "Serial.h"
#include "Process.h"
//#include "TcpSocketServer.h"
#include "Config.h"


Process::Process() :
    Timer()
//    ,TcpSocketServer(cfg->tcpPort)
{
    curProgramId = 0;
    lastProgramId = 0;

    ledsLen = cfg->ledsLen;

    bufCmdMega = new Proto(cfg->addrFrom,cfg->addrTo);
    bufCmdLeds = new Proto(cfg->addrFrom,cfg->addrTo);

    ledsVal = new int(ledsLen);

    dio_last = 0;
    dio = 0;

    tvpActive = true;
}

Process::~Process()
{
    delete [] ledsVal;
    delete bufCmdLeds;
    delete bufCmdMega;
    delete cr;
    delete cfg;
    delete mega;
    delete leds;
}

#define CICLELEN 60

void Process::setPhase(int _phase)
{
    mega->SendCmd(bufCmdMega->setPhase(_phase), BUFSIZE, bufCmdMega->len());
}
//===========================================================================================================

int Process::getProgrmId(int toTime)
{
    //save last programm id
    lastProgramId = curProgramId;

    curProgramId = Cron::getProgramId(toTime);
    //was tvp reaction
    if(lastProgramId != curProgramId)
    {
        return curProgramId;
    }

    if(!tvp.empty())
    {
        for(unsigned i = 0; i < cfg->Programs.size(); i++)
        {
            if(cfg->Programs[i]->tvpId == tvp.front())
            {
                curProgramId = cfg->Programs[i]->Id;
                break;
            }
        }

        //del queue
        while (tvp.size() > 0)
            tvp.pop();

    }
    printf("set program Id: %d\n",curProgramId);
    return curProgramId;
}
//===========================================================================================================

void Process::setNextPhase()
{
    setPhase(cfg->Programs[curProgramId]->getFirstPhase());
}

//===========================================================================================================
int Process::run()
{
    mega = new Serial(cfg->portMega, cfg->speedMega);
    mega->Config();

    leds = new Serial(cfg->portLeds, cfg->speedLeds);
    leds->Config();

    shm = new ShMem(false);
    dio_last = shm->read();

    mega->SendCmd(bufCmdMega->setTime(), BUFSIZE, bufCmdMega->len());

    //TcpSocketServer::start();
    getProgrmId(0);
    Timer::start();

    for(;;)
    {
        cfg->Programs[curProgramId]->run(this);
        tvpActive = true;
        //wait a second
        WaitSec();
    }
    return 0;
}
//===========================================================================================================
bool Process::cmd()
{
    /*
        if(TcpSocketServer::buf[0] == 0x3A)
        {
            lock();
            if( mega->SendCmd(TcpSocketServer::buf, sizeof(TcpSocketServer::buf), 6 + TcpSocketServer::buf[4]) )
            {
                log->debug("tty read: %d", TcpSocketServer::buf[4]);
            }
            unlock();
        }
        else if(TcpSocketServer::buf[0]== 0x3B)//not asudd proto
        {
            log->info("custom cmd: %d", TcpSocketServer::buf[1]);

            switch(TcpSocketServer::buf[1])
            {
            case 0x01:
                TcpSocketServer::buf[1] = phase;
                break;
            case 0x02:
                TcpSocketServer::buf[1] = phase;
    //            cr->phaseFix(TcpSocketServer::buf[2]);
                break;
            }
        }*/
    return true;
}
//===========================================================================================================
void Process::setTvpVal()
{
    if(!tvpActive)
    {
        return;
    }

    dio = shm->read();
    if(dio_last == dio)
    {
        return;
    }
    dio_last = dio;

    printf("dio: %X\n",dio);

    for(auto it = cfg->masks.begin(); it != cfg->masks.end(); ++it)
    {
        if( !(dio & (*it).second) )
        {
            printf("queue: push: %d dio:%X mask:%X\n", (*it).first , dio, (*it).second);
            if(tvp.size() > MAX_CMD_QUEUE_LEN)
            {
                for(unsigned int i= 0; i < tvp.size(); i++)
                {
                    if(i != tvp.size() - 1)
                        tvp.push( tvp.front() );
                    tvp.pop();
                }
            }

            tvp.push((*it).first);
            //break;
        }
    }
}

//===========================================================================================================
void Process::WaitSec()
{
    for(int i = 0; i < 5; i++)
    {
        Timer::wait();
    }

    printf("led-%d: ",ledsLen);
    for(int i = 0; i < ledsLen; i++)
    {
        printf("%d:%u ",i,ledsVal[i]);
    }
    printf("\n");
    fflush(stdout);
}
//===========================================================================================================
void Process::AlarmFunction()
{
    leds->SendCmdWO(bufCmdLeds->cmd_Tab(ledsVal,ledsLen),42);
    setTvpVal();
}
