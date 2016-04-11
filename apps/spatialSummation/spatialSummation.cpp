#include <iostream>
#include <unistd.h>
#include <time.h>
#include <yaml-cpp/yaml.h>

#include <lgnSimulator.h>

using namespace std;
using namespace lgnSimulator;

int main(int argc, char* argv[])
{

    cout << "=====LGN Simulator Model: Spatial summation =====" << endl;
    clock_t t;
    t = clock();

    if(argc < 2) {
        cerr << "Too few arguments." << endl;
        return 1;
    }


    //read config file-------------------------------------------------------
    YAML::Node cfg = YAML::LoadFile(argv[1]);

    //Output manager:---------------------------------------------------------
    OutputManager io(cfg["OutputManager"]["outputFilename"].as<std::string>());

    //Integrator--------------------------------------------------------------
    Integrator integrator = createIntegrator(cfg["grid"]);
    io.writeIntegratorProperties(integrator);

    //Stim---------------------------------------------------------------------
    unique_ptr<Grating> S = createGratingStimulus(integrator, cfg["stimulus"]);

    //Ganglion cell:-----------------------------------------------------------
    DOG Wg_s = createSpatialDOGKernel(cfg["ganglion"]["Wg"]);
    Biphasic Wg_t = createTemporalBiphasicKernel(cfg["ganglion"]["Wt"]);

    SeparableKernel Wg(&Wg_s, &Wg_t);
    GanglionCell ganglion(integrator, Wg, cfg["ganglion"]["R0"].as<double>());


    //Relay cell: -------------------------------------------------------------
    RelayCell relay(integrator, cfg["relay"]["R0"].as<double>());

    // G -> R
    SpatialDelta Ks_rg = createSpatialDeltaKernel(cfg["relay"]["Krg"]["spatial"]);
    TemporalDelta Kt_rg = createTemporalDeltaKernel(cfg["relay"]["Krg"]["temporal"]);
    SeparableKernel Krg(&Ks_rg, &Kt_rg);

    // C -> R
    SpatialGaussian Ks_rc = createSpatialGaussianKernel(cfg["relay"]["Krc"]["spatial"]);
    TemporalDelta Kt_rc = createTemporalDeltaKernel(cfg["relay"]["Krc"]["temporal"]);

    //    EllipticGaussian Ks_rc = createSpatialEllipticGaussianKernel(cfg["relay"]["Krc"]["spatial"]));
    //    DOE Kt_rc = createTemporalDOEKernel(cfg["relay"]["Krc"]["temporal"]);

    SeparableKernel Krc(&Ks_rc, &Kt_rc);

    //Cortical cell: -------------------------------------------------------------
    CorticalCell cortical(integrator, cfg["cortical"]["R0"].as<double>());

    // R -> G
    SpatialDelta Ks_cr = createSpatialDeltaKernel(cfg["cortical"]["Kcr"]["spatial"]);
    TemporalDelta Kt_cr = createTemporalDeltaKernel(cfg["cortical"]["Kcr"]["temporal"]);
    //    DOE Kt_cr = createTemporalDOEKernel(cfg["cortical"]["Kcr"]["temporal"]);

    SeparableKernel Kcr(&Ks_cr, &Kt_cr);


    //Connect neurons:---------------------------------------------------------
    relay.addGanglionCell(&ganglion, Krg);
    relay.addCorticalNeuron(&cortical, Krc);
    cortical.addRelayCell(&relay, Kcr);

    //Compute:-----------------------------------------------------------------

    if(cfg["stimulus"]["storeSpatiotemporal"].as<bool>()){
        S->computeSpatiotemporal();
        io.writeStimulus(S.get(),cfg["stimulus"]["storeSpatiotemporalFT"].as<bool>());
        S->clearSpatioTemporal();
    }
    S->computeFourierTransform();
    ganglion.computeImpulseResponseFourierTransform();
    relay.computeImpulseResponseFourierTransform();
    cortical.computeImpulseResponseFourierTransform();



    if(cfg["ganglion"]["storeResponse"].as<bool>()){
        ganglion.computeResponse(S.get());
        io.writeResponse(ganglion, cfg["ganglion"]["storeResponseFT"].as<bool>());
        ganglion.clearResponse();
    }

    if( cfg["ganglion"]["storeImpulseResponse"].as<bool>()){
        ganglion.computeImpulseResponse();
        io.writeImpulseResponse(ganglion,
                                cfg["ganglion"]["storeImpulseResponseFT"].as<bool>());
        ganglion.clearImpulseResponse();
    }

    if(cfg["relay"]["storeResponse"].as<bool>()){
        relay.computeResponse(S.get());
        io.writeResponse(relay,
                         cfg["relay"]["storeResponseFT"].as<bool>());
        relay.clearResponse();
    }

    if( cfg["relay"]["storeImpulseResponse"].as<bool>()){
        relay.computeImpulseResponse();
        io.writeImpulseResponse(relay,
                                cfg["relay"]["storeImpulseResponseFT"].as<bool>());
        relay.clearImpulseResponse();
    }


    if(cfg["cortical"]["storeResponse"].as<bool>()){
        cortical.computeResponse(S.get());
        io.writeResponse(cortical,
                         cfg["cortical"]["storeResponseFT"].as<bool>());
        cortical.clearResponse();
    }

    if( cfg["cortical"]["storeImpulseResponse"].as<bool>()){
        cortical.computeImpulseResponse();
        io.writeImpulseResponse(cortical,
                                cfg["cortical"]["storeImpulseResponseFT"].as<bool>());
        cortical.clearImpulseResponse();
    }



    //Finalize:----------------------------------------------------------
    t = clock() - t;
    double elapsedTime = ((float)t)/CLOCKS_PER_SEC;
    if(elapsedTime <= 60){
        printf ("%f seconds.\n", elapsedTime);
    }else{
        printf ("%f minutes.\n", elapsedTime/60);
    }


    return 0;
}

