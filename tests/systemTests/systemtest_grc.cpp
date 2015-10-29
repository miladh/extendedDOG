#include <unittest++/UnitTest++.h>

#include "neurons/ganglioncell.h"
#include "neurons/relaycell.h"
#include "neurons/corticalcell.h"

#include "stimuli/grating.h"
#include "integrator.h"

#include "../tests/systemTests/kernelsettings.h"

SUITE(SYSTEM){

    TEST(ganglion_relay_cortical){
        int ns = 5;
        int nt = 4;
        double ds = 0.1;
        double dt = 0.2;


        int Ns = pow(2,ns);
        int Nt = pow(2,nt);

        cube Rg = zeros<cube>(Ns, Ns, Nt);
        cube Rg_ex = zeros<cube>(Ns, Ns, Nt);
        cube Rr = zeros<cube>(Ns, Ns, Nt);
        cube Rr_ex = zeros<cube>(Ns, Ns, Nt);
        cube Rc = zeros<cube>(Ns, Ns, Nt);
        cube Rc_ex = zeros<cube>(Ns, Ns, Nt);

        //Integrator
        Integrator integrator(nt, dt, ns, ds);
        vec s = integrator.coordinateVec();
        vec k = integrator.spatialFreqVec();
        vec t = integrator.timeVec();
        vec w = integrator.temporalFreqVec();

        //Stimulus
        double C = -2.3;
        double wd = w(0);
        double kx = k(0);
        double ky = k(0);
        Grating S(&integrator, {kx, ky}, wd, C);
        S.computeFourierTransform();


        //Kernels
        vector<SpatialKernel*> spatialKernels = KernelSettings::spatialKernelVector();
        vector<TemporalKernel*> temporalKernels = KernelSettings::temporalKernelVector();



        for(SpatialKernel* Fs : spatialKernels){
            for(TemporalKernel* Ft : temporalKernels){

                //ganglion cell
                GanglionCell ganglion(&integrator, Fs, Ft);
                double Wg = Fs->fourierTransform({kx, ky}) * Ft->fourierTransform(wd);


                //Compute ganglion response numerical
                ganglion.computeResponse(&S);
                Rg = ganglion.response();

                //Compute ganglion response analytic
                for(int l = 0; l < Nt; l++){
                    for(int i = 0; i < Ns; i++){
                        for(int j = 0; j < Ns; j++){
                            Rg_ex(i,j,l) = C * Wg *
                                    cos(kx*s[i] + ky*s[j] - wd * t[l]);
                        }
                    }
                }

                for(SpatialKernel* Ks_rg : spatialKernels){
                    for(TemporalKernel* Kt_rg : temporalKernels){

                        double Krg =  Ks_rg->fourierTransform({kx, ky})
                                * Kt_rg->fourierTransform(wd);

                        for(SpatialKernel* Ks_rc : spatialKernels){
                            for(TemporalKernel* Kt_rc : temporalKernels){

                                //cells
                                RelayCell relay(&integrator);
                                CorticalCell cortical(&integrator);

                                //connect cells
                                relay.addGanglionCell(&ganglion, Ks_rg, Kt_rg);
                                relay.addCorticalNeuron(&cortical,  Ks_rc, Kt_rc);
                                cortical.addRelayCell(&relay, Ks_rc, Kt_rc);


                                double Krc =  Ks_rc->fourierTransform({kx, ky})
                                        * Kt_rc->fourierTransform(wd);
                                double Kcr =  Krc;

                                //Compute analytic:
                                double Wr = Krg * Wg/(1. -  Krc * Kcr);
                                double Wc = Kcr * Wr;


                                for(int l = 0; l < Nt; l++){
                                    for(int i = 0; i < Ns; i++){
                                        for(int j = 0; j < Ns; j++){
                                            Rr_ex(i,j,l) = C * Wr *
                                                    cos(kx*s[i] + ky*s[j] - wd * t[l]);
                                            Rc_ex(i,j,l) = C * Wc *
                                                    cos(kx*s[i] + ky*s[j] - wd * t[l]);

                                        }
                                    }
                                }

                                //Compute numerical
                                relay.computeResponse(&S);
                                cortical.computeResponse(&S);
                                Rr = relay.response();
                                Rc = cortical.response();



                                // Test
                                for(int l = 0; l < Nt; l++){
                                    for(int i = 0; i < Ns; i++){
                                        for(int j = 0; j < Ns; j++){
                                            CHECK_CLOSE(Rr_ex(i,j,l), Rr(i,j,l), 1e-12);
                                            CHECK_CLOSE(Rc_ex(i,j,l), Rc(i,j,l), 1e-12);

                                        }
                                    }
                                }

                            }//End of Kt_rc loop
                        }//End of Ks_rc loop

                    }//End of Kt_rg loop
                }//End of Ks_rg loop


                // Test
                for(int l = 0; l < Nt; l++){
                    for(int i = 0; i < Ns; i++){
                        for(int j = 0; j < Ns; j++){
                            CHECK_CLOSE(Rg_ex(i,j,l), Rg(i,j,l), 1e-12);

                        }
                    }
                }

            }//End of Ft loop
        }//End of Fs loop

    }

}



