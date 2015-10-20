#ifndef GRATING_H
#define GRATING_H

#include "stimuli.h"


class Grating : public Stimuli
{
public:
    Grating(Integrator integrator, vec2 kd, double wd, double contrast);
    ~Grating();

    // Stimuli interface
private:
    double m_contrast = 0.0;
    vec2 m_k = {0,0};
    double m_w = 0;

    double valueAtPoint(vec2 rVec, double t);
    double fourierTransformAtFrequency(vec2 k, double w);

};

#endif // GRATING_H
