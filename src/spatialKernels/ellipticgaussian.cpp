#include "ellipticgaussian.h"

EllipticGaussian::EllipticGaussian(double weight, double angle,
                                   double widthLong, double widthNarrow)
    : m_weight(weight)
    , m_angle(angle*PI/180.)
    , m_widthLong(widthLong)
    , m_widthNarrow(widthNarrow)
    , m_cosTheta(cos(m_angle))
    , m_sinTheta(sin(m_angle))

{

}

EllipticGaussian::~EllipticGaussian()
{

}

double EllipticGaussian::spatial(vec2 rVec)
{

    double exp1 = (rVec[0] * m_cosTheta + rVec[1] * m_sinTheta)/m_widthLong;
    double exp2 = (rVec[1] * m_cosTheta - rVec[0] * m_sinTheta)/m_widthNarrow;
    return m_weight/PI/m_widthNarrow/m_widthLong * exp(-exp1*exp1 - exp2*exp2);
}

double EllipticGaussian::fourierTransform(vec2 kVec)
{
    double exp1 = (kVec[0] * m_cosTheta + kVec[1] * m_sinTheta)*m_widthLong/2;
    double exp2 = (kVec[1] * m_cosTheta - kVec[0] * m_sinTheta)*m_widthNarrow/2;

    return m_weight * exp(-exp1*exp1 - exp2*exp2);
}



EllipticGaussian createEllipticGaussianSpatialKernel(const Config *cfg)
{
    const Setting & root = cfg->getRoot();
    double weight = root["spatialKernelSettings"]["weight_elliptic"];
    double angle = root["spatialKernelSettings"]["angle"];
    double widthLong = root["spatialKernelSettings"]["widthLong"];
    double widthNarrow = root["spatialKernelSettings"]["widthNarrow"];

    return EllipticGaussian(weight, angle, widthLong, widthNarrow);
}
