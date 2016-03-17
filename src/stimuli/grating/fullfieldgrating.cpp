#include "fullfieldgrating.h"

using namespace lgnSimulator;


FullFieldGrating::FullFieldGrating(const Integrator &integrator,
                                    double spatialFreq, double orientation,
                                   double  temporalFreq,double contrast)
    : Grating(integrator, spatialFreq, orientation, temporalFreq, contrast, 0)
{
    m_mask = "none";
    m_peak = 1.0
            / m_integrator.temporalFreqResolution()
            / m_integrator.spatialFreqResolution()
            / m_integrator.spatialFreqResolution();

}

FullFieldGrating::~FullFieldGrating()
{
}

double FullFieldGrating::valueAtPoint(vec2 rVec, double t) const
{

    double s = m_contrast * cos(dot(m_kVec, rVec) - m_w * t);
    return s;
}

complex<double> FullFieldGrating::fourierTransformAtFrequency(vec2 k, double w) const
{
    double s = (Special::delta(k, m_kVec) * Special::delta(w, m_w)
                + Special::delta(k, -m_kVec) * Special::delta(w, -m_w));

    return 4.*core::pi*core::pi*core::pi * m_contrast * m_peak * s;
}


