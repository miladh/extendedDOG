#include "fullfieldgrating.h"

FullFieldGrating::FullFieldGrating(Integrator *integrator,
                                   vec2 kd, double wd, double contrast)
    : Grating(integrator, kd, wd, contrast, 0)
{

}

FullFieldGrating::~FullFieldGrating()
{

}


double FullFieldGrating::valueAtPoint(vec2 rVec, double t)
{
    double s = m_contrast * cos(dot(m_k, rVec) - m_w * t);
    return s;
}

double FullFieldGrating::fourierTransformAtFrequency(vec2 k, double w)
{
    double s = Functions::delta(k[0], m_k[0])
            * Functions::delta(k[1], m_k[1])
            * Functions::delta(-w, m_w)
            /m_integrator->spatialFreqResolution()
            /m_integrator->spatialFreqResolution()
            /m_integrator->temporalFreqResolution();

    return 8*PI*PI*PI* m_contrast * s;
}


FullFieldGrating createFullFieldGratingStimulus(Integrator *integrator, const Config *cfg)
{
    const Setting & root = cfg->getRoot();
    double contrast = root["stimuliSettings"]["GratingSettings"]["C"];

    vec k = integrator->spatialFreqVec();
    vec w = integrator->temporalFreqVec();
    double wd = w(1);
    double kx = k(7);
    double ky = k(0);


    return FullFieldGrating(integrator, {kx, ky}, wd, contrast);
}

