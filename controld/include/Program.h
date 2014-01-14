#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>

#include "ProgramPhase.h"
#include "Phase.h"

class Process;

class Program
{
    typedef std::vector<ProgramPhase*>::iterator pPhase;
    public:
        int Id;
        int tvpId;
        std::vector<ProgramPhase*> vPhases;

        Program();
        virtual ~Program();
        Program(const Program *c)
        {
            Id = c->Id;
            Len = 0;
            for( auto it = c->vPhases.begin(); it != c->vPhases.end(); ++it)
            {
                vPhases.push_back(new ProgramPhase(*it));
                Len += (*it)->Tocn;
            }

            curPhase = vPhases.begin();
            nextPhase = curPhase + 1;
            curPhaseTime = 0;
        }

        void run(Process *p);

        void addPhase(Phase *ph, int time)
        {
            vPhases.push_back(new ProgramPhase(ph, time));
            Len += time;
        }

        void setPhase(int phId)
        {
            int i = 1;
            for( auto it = vPhases.begin(); it != vPhases.end(), i < phId; ++it, i++)
            {
                curPhase = it;
            }
            curPhaseTime = 0;
        }

        int Tc(){ return Len; }
        int Count(){ return Len - endOfProgram; }
        int getFirstPhase();

    protected:
    private:
        pPhase curPhase, nextPhase;
        int curPhaseTime, Len, endOfProgram, phaseCount;

        int phaseSwitchWayTime(int wayNumber);
        void countDownWays(int *ledsVal, int ledsLen);
        void countDownWaysInit(int *ledsVal, int ledsLen);
};

#endif // PROGRAM_H
