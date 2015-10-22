#ifndef RELAYCELL_H
#define RELAYCELL_H

#include "neuron.h"

class RelayCell : public Neuron
{
public:
    RelayCell(Integrator *integrator);
    ~RelayCell();

protected:
    // Neuron interface
    double impulseResponseFT(vec2 kVec, double w);
};

#endif // RELAYCELL_H
