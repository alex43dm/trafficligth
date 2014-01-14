#include <stdio.h>
#include "ProgramPhase.h"

ProgramPhase::ProgramPhase(Phase *ph, int time) : Phase(ph)
{
    Tocn = time;
    Tpromt = Tocn - PromPulse;
}

ProgramPhase::ProgramPhase(const ProgramPhase *o)
{
    Id = o->Id;
    Tmin = o->Tmin;
    Tmax = o->Tmax;
    TVP = o->TVP;
    PromPulse = o->PromPulse;
    Tocn = o->Tocn;
    Tpromt = o->Tpromt;
    Ways.insert(Ways.begin(),o->Ways.begin(), o->Ways.end());
}

ProgramPhase::~ProgramPhase()
{
    //dtor
}
