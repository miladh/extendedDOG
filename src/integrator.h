#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <iostream>
#include <armadillo>
#include <math.h>
#include <fftw3.h>

#include "integratorsettings.h"

using namespace std;
using namespace arma;

class Integrator
{
public:
    Integrator(IntegratorSettings *settings);
    ~Integrator();

    cx_cube integrate(cx_cube data);

    vec timeVec() const;
    vec coordinateVec() const;
    vec temporalFreqVec() const;
    vec spatialFreqVec() const;

private:
    IntegratorSettings *m_settings;

    int m_nPointsTemporal= 0;
    int m_nPointsSpatial = 0;
    double m_maxT = 0;

    double m_dt = 0;
    double m_dw = 0;
    double m_temporalSamplingFreq = 0;

    double m_ds = 0;
    double m_dk = 0;
    double m_spatialSamplingFreq = 0;

    vec m_timeVec;
    vec m_coordinateVec;
    vec m_temporalFreqVec;
    vec m_spatialFreqVec;




};

#endif // INTEGRATOR_H
