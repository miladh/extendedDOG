#ifndef SPECIAL_H
#define SPECIAL_H


#include <boost/math/special_functions/bessel.hpp>
#include <boost/tr1/cmath.hpp>
#include <helper/helperconstants.h>
#include <armadillo>
#include <gsl/gsl_sf_hyperg.h>

using namespace arma;

namespace lgnSimulator {
class Special
{
public:
    Special();
    ~Special();
    static double heaviside(double x);
    static unsigned int factorial(unsigned int n);
    static double secondKindBessel(double x);
    static double confluentHypergeometric(double a, double b, double x);
    static int isOdd(int num);
    static double delta(double x, double y);
    static double delta(vec2 x, vec2 y);
    static double nearestValue(const vec x, const double value);

};
}
#endif // SPECIAL_H
