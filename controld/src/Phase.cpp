#include "Phase.h"

Phase::Phase()
{

}

Phase::Phase(const Phase *h)
{
    Id          = h->Id;
    Tmin        = h->Tmin;
    Tmax        = h->Tmax;
    TVP         = h->TVP;
    PromPulse   = h->PromPulse;
    Ways.clear();
    Ways.insert(Ways.begin(),h->Ways.begin(), h->Ways.end());
}

Phase::~Phase()
{
    //dtor
    Ways.clear();
}
