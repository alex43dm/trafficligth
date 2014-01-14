#ifndef PROGRAMPHASE_H
#define PROGRAMPHASE_H

#include "Phase.h"

class ProgramPhase : public Phase
{
    public:
        int Tocn;
        int Tpromt;
        ProgramPhase(Phase *ph, int time);
        ProgramPhase(const ProgramPhase *);
        virtual ~ProgramPhase();
    private:
};

#endif // PROGRAMPHASE_H
