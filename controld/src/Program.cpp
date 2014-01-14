#include "Program.h"
#include "Log.h"
#include "Phase.h"
#include "Process.h"

Program::Program()
{
    Len = 0;
    Id = 0;
    tvpId = 0;
}

Program::~Program()
{
}

int Program::phaseSwitchWayTime(int wayNumber)
{
    //clear val
    int ledsVal = 0;
    printf("\ncur phase: %02d way:%02d\n", (*curPhase)->Id, wayNumber);
    for( pPhase curPh = curPhase, nextPh = curPhase; curPh != vPhases.end(); ++curPh)
    {
        if(++nextPh == vPhases.end())
        {
            nextPh = vPhases.begin();
        }

        if((*curPh)->Ways[wayNumber]->state == on && (*nextPh)->Ways[wayNumber]->state == off)//close way
        {
            return ledsVal + (*curPh)->Tocn;
        }
        else if((*curPh)->Ways[wayNumber]->state == off && (*nextPh)->Ways[wayNumber]->state == on)//open way
        {
            if((*curPh)->Ways[wayNumber]->type == WayTypes::People)
            {
                return ledsVal + (*curPh)->Tocn + (*nextPh)->PromPulse;//6..3
            }
            else if((*curPh)->Ways[wayNumber]->type == WayTypes::Tranport)
            {
                return ledsVal + (*curPh)->Tocn;
            }
        }
        else//stay
        {
            ledsVal += (*curPh)->Tocn;

            if( ledsVal >= endOfProgram )
            {
                return 0;//condom
            }
        }
    }
    return ledsVal;
}

void Program::countDownWays(int *ledsVal, int ledsLen)
{
    printf("p:%02d c:%02d-%02d p:%02d:%02d ", Id, endOfProgram, Len-endOfProgram, (*curPhase)->Id, phaseCount);
    for(int i = 0; i < ledsLen; i++)
    {
        ledsVal[i] > 0 ? ledsVal[i]-- : ledsVal[i] = 0;
    }
}


void Program::countDownWaysInit(int *ledsVal, int ledsLen)
{
    for(int i = 0; i < ledsLen; i++)
    {
        ledsVal[i] = phaseSwitchWayTime(i);
    }
}

int Program::getFirstPhase()
{
    return (*vPhases.begin())->Id;
}

void Program::run(Process *proc)
{
    int i;

    endOfProgram = Len;

    for(curPhase = vPhases.begin(), nextPhase = curPhase; curPhase != vPhases.end(); ++curPhase)
    {
        if(++nextPhase == vPhases.end())
        {
            nextPhase = vPhases.begin();
        }

        phaseCount = (*curPhase)->Tocn + (*nextPhase)->PromPulse;
        //way init
        countDownWaysInit(proc->ledsVal, proc->ledsLen);
//        if(curPhase != vPhases.begin())
//        {
            for(i = (*curPhase)->PromPulse; i > 0 ; i--, phaseCount--, endOfProgram--)
            {
                proc->setPhase((*curPhase)->Id);
                countDownWays(proc->ledsVal, proc->ledsLen);
                proc->WaitSec();
            }
//        }

        for(i = (*curPhase)->Tocn - (*curPhase)->PromPulse; i > 0;  i--, phaseCount--, endOfProgram--)
        {
                proc->setPhase((*curPhase)->Id);
                countDownWays(proc->ledsVal, proc->ledsLen);
                proc->WaitSec();

                if( nextPhase == vPhases.begin()
                   && proc->isTVPpressed()
                   && (*curPhase)->Tocn - (*curPhase)->PromPulse - i >= (*curPhase)->Tmin)
                {
                    break;
                }
        }

        //select the next program id will be run
        if(nextPhase == vPhases.begin())
        {
            proc->getProgrmId((*curPhase)->PromPulse);
        }

        for(i = (*curPhase)->PromPulse; i > 0 ; i--, phaseCount--, endOfProgram--)
        {
            if(nextPhase != vPhases.begin())
            {
                proc->setPhase((*nextPhase)->Id);
            }
            else//
            {
                proc->setNextPhase();
            }

            countDownWays(proc->ledsVal, proc->ledsLen);
            proc->WaitSec();
        }

    }

    return;
}
