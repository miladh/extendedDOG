#ifndef SPECIAL_H
#define SPECIAL_H


#include <boost/math/special_functions/bessel.hpp>
#include <helper/helperconstants.h>
#include <armadillo>

using namespace arma;

namespace lgnSimulator {
class Special
{
public:
    Special();
    ~Special();
    static double heaviside(double x);
    static double secondKindBesselFunction(double x);
    static int isOdd(int num);
    static double delta(double x, double y);
    static double delta(vec2 x, vec2 y);
    static double nearestValue(const vec x, const double value);

};
}
#endif // SPECIAL_H
