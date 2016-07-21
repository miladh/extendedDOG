/**********************************************************************
 *  Test: response of ganglion cell and interneuron with full-field
 *        grating stimulus:
 *               Rg(r,t) = W(r,t) * S(r,t)
 *               Ri(r,t) = [W(r,t)K(r,t)] * S(r,t)
 *
 *  Analytic source: closed-form experssion
 *
 * ********************************************************************/
#include <lgnSimulator.h>
#include <catch.hpp>


using namespace lgnSimulator;

void runSystemTest_GI(int nt, double dt, int ns, double ds,
                      double C, int wdId, int kxId, int thetaId,
                      const Kernel &W, const Kernel &K)
{

    //integrator
    Integrator integrator(nt, dt, ns, ds);
    vec r = integrator.spatialVec();
    vec k = integrator.spatialFreqVec();
    vec t = integrator.timeVec();
    vec w = integrator.temporalFreqVec();
    vec orientations = vec{0., 90., 180., 270, -360.};


    //stimulus
    double wd = w(wdId);
    double spatialFreq = k(kxId);
    double orientation = orientations(thetaId);

    double phase = 0.0;

    FullFieldGrating grating(&integrator, spatialFreq, wd, C, phase, orientation);
    grating.computeFourierTransform();

    //ganglion cell
    GanglionCell ganglion(&integrator, W);
    ganglion.computeResponse(&grating);

    //interneuron cell
    Interneuron interneuron(&integrator);
    interneuron.addGanglionCell(&ganglion, K);
    interneuron.computeResponse(&grating);

    cube Rg_e = zeros(r.n_elem, r.n_elem, t.n_elem);
    cube Ri_e = zeros(r.n_elem, r.n_elem, t.n_elem);

    double kx = spatialFreq*cos(orientation*core::pi/180.);
    double ky = spatialFreq*sin(orientation*core::pi/180.);
    complex<double> Wijl = W.fourierTransform({kx, ky}, wd);
    complex<double> Kijl = Wijl* K.fourierTransform({kx, ky}, wd);

    for(int l=0; l < int(t.n_elem); l++){
        for(int i = 0; i < int(r.n_elem); i++){
            for(int j = 0; j < int(r.n_elem); j++){
                Ri_e(i,j,l) = C * abs(Kijl)
                        * cos(kx*r[i] + ky*r[j] - wd * t[l] + arg(Kijl));
                Rg_e(i,j,l) = C * abs(Wijl)
                        * cos(kx*r[i] + ky*r[j] - wd * t[l] + arg(Wijl));
            }
        }
    }

    cube diff_g = abs(Rg_e - ganglion.response());
    cube diff_i = abs(Ri_e - interneuron.response());
    REQUIRE(diff_g.max()== Approx(0.0).epsilon(1e-10));
    REQUIRE(diff_i.max()== Approx(0.0).epsilon(1e-10));

}

\

TEST_CASE("runSystemTest_GI_0"){
    double dt = 0.1;
    double weight = 1;
    SpatialGaussian Ws( 0.25);
    TemporalDelta Wt(0, dt);
    SeparableKernel W(weight, &Ws, &Wt);

    runSystemTest_GI(2, dt, 2, 0.1,
                     -1, 0, 1, 0, W, W);
}


TEST_CASE("runSystemTest_GI_1"){
    double dt = 0.1;
    double weight_w = 1;
    SpatialGaussian Ws(0.25);
    TemporalDelta Wt(1*dt, dt);
    SeparableKernel W(weight_w, &Ws, &Wt);

    double weight_k = -1;
    SpatialGaussian Ks(0.55);
    TemporalDelta Kt(2*dt, dt);
    SeparableKernel K(weight_k, &Ws, &Wt);

    runSystemTest_GI(3, dt, 2, 0.1,
                     -1, 3, 1, 4, W, K);
}



