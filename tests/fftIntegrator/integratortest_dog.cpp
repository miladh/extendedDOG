#include <unittest++/UnitTest++.h>
#include <armadillo>
#include <iostream>

#include "integrator.h"
#include "spatialKernels/dog.h"


using namespace std;
using namespace arma;


SUITE(INTEGRATOR){

    TEST(dogfft){
        //Mesh
        int ns = 9;
        double ds = 0.1;
        double A = 1.0;
        double a = 2.1;
        double B = 1.0;
        double b = 3.1;

        Integrator integrator(0, 0, ns, ds);

        int Ns = pow(2,ns);
        cx_mat g = zeros<cx_mat>(Ns, Ns);
        cx_mat G = zeros<cx_mat>(Ns, Ns);
        cx_mat f = zeros<cx_mat>(Ns, Ns);

        vec s = integrator.coordinateVec();
        vec k = integrator.spatialFreqVec();

        DOG dog(A, a, B, b);

        for(int i = 0; i < Ns; i++){
            for(int j = 0; j < Ns; j++){
                g(i,j) = dog.spatial({s[i], s[j]});
            }
        }

        //fourier signal
        for(int i = 0; i < Ns; i++){
            for(int j = 0; j < Ns; j++){
                f(i,j) = dog.fourierTransform({k[i], k[j]});
            }
        }

        // Backward
        G = integrator.integrate(f);

        for(int i = 0; i < Ns; i++){
            for(int j = 0; j < Ns; j++){
                CHECK_CLOSE(real(g(i,j)),
                            real(G(i,j)), 1e-9);

            }
        }
    }
}


