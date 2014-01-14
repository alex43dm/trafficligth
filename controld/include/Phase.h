#ifndef PHASE_H
#define PHASE_H

#include <vector>

#include "Way.h"

typedef std::vector<Way *>::iterator pWay;

class Phase
{
public:
    int Id;
    int Tmin;
    int Tmax;
    bool TVP;
    int PromPulse;
    std::vector<Way *> Ways;

    Phase();
    Phase(const Phase *h);
    virtual ~Phase();

protected:
private:
};

#endif // PHASE_H
