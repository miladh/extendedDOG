#ifndef FULLFIELDGRATING_H
#define FULLFIELDGRATING_H

#include "grating.h"


namespace lgnSimulator {
class FullFieldGrating : public Grating
{
public:
    FullFieldGrating(const Integrator &integrator, double spatialFreq,
                     double orientation, double temporalFreq, double contrast);
    ~FullFieldGrating();

    // Grating interface
private:
    virtual double valueAtPoint(vec2 r, double t) const override;
    virtual complex<double> fourierTransformAtFrequency(vec2 k, double w) const override;

    double m_peak = 1.;
};

}
#endif // FULLFIELDGRATING_H
