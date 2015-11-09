#include <iostream>

#include <outputmanager.h>
#include <unistd.h>

#include "stimuli/patchgrating.h"
#include "stimuli/grating.h"
#include "stimuli/oscillatinggaussian.h"
#include "stimuli/staticimage.h"
#include "stimuli/naturalscenevideo.h"

#include "integrator.h"

#include "neurons/ganglioncell.h"
#include "neurons/interneuron.h"
#include "neurons/relaycell.h"
#include "neurons/corticalcell.h"

#include "spatialKernels/dog.h"
#include "spatialKernels/ellipticgaussian.h"

#include "temporalKernels/decayingexponential.h"
#include "temporalKernels/dampedoscillator.h"
#include "temporalKernels/temporallyconstant.h"
#include "temporalKernels/temporaldelta.h"


using namespace std;

int main()
{

    cout << "=====Extended-DOG Model=====" << endl;

    //read config file-------------------------------------------------------
    Config cfg;
    cfg.readFile("../../eDOG/app/config.cfg");

    //Integrator-------------------------------------------------------------
    Integrator integrator = createIntegrator(&cfg);

    //Stim---------------------------------------------------------------------
//    NaturalSceneVideo S = createNaturalSceneVideoStimulus(&integrator,&cfg);
    StaticImage S = createStaticImageStimulus(&integrator,&cfg);
//    Grating S = createGratingStimulus(&integrator,&cfg);
//    PatchGrating S = createPatchGratingStimulus(&integrator,&cfg);
//    OscillatingGaussian S = createOscillatingGaussianStimulus(&integrator,&cfg);



    //Spatial kernels:----------------------------------------------------------
    DOG dog = createDOGSpatialKernel(&cfg);
    DOG gauss(1.0, 2.5, 0, 0.1);
//    EllipticGaussian ellipticGauss = createEllipticGaussianSpatialKernel(&cfg);


    //Temporal kernels:-------------------------------------------------------
    DecayingExponential Kt_rg(21,0);
    DecayingExponential Kt_rig(30,0);
    DecayingExponential Kt_rc(42,10);
    TemporalDelta Kt_cr(20);
    DampedOscillator damped = createDampedOscillatorTemporalKernel(&cfg);
//    TemporallyConstant tempConst = createTemporallyConstantTemporalKernel(&cfg);


    //Neurons:-----------------------------------------------------------------
    GanglionCell ganglion(&integrator, &dog, &damped);
    RelayCell relay(&integrator);
    CorticalCell cortical(&integrator);
//    Interneuron interneuron(&integrator);

    vector<Neuron *> neurons;
    neurons.push_back(&ganglion);
    neurons.push_back(&relay);
    neurons.push_back(&cortical);
//    neurons.push_back(&interneuron);



    //connect neurons----------------------------------------------------------
    relay.addGanglionCell(&ganglion,&dog, &Kt_rg);
    relay.addGanglionCell(&ganglion,&dog, &Kt_rig);
    relay.addCorticalNeuron(&cortical, &gauss, &Kt_rc);
//    relay.addInterNeuron(&interneuron,&gauss, &damped);

//    interneuron.addGanglionCell(&ganglion,&gauss, &Kt);
//    interneuron.addCorticalNeuron(&cortical, &ellipticGauss, &Kt);

    cortical.addRelayCell(&relay, &dog, &Kt_cr);



    //Compute:-----------------------------------------------------------------
    S.computeSpatiotemporal();
    S.computeFourierTransform();

    ganglion.computeResponse(&S);
    relay.computeResponse(&S);
    cortical.computeResponse(&S);
//    interneuron.computeResponse(&S);





    //Output manager:----------------------------------------------------------
    OutputManager io(&cfg);
    io.writeResponse(neurons, S);


    return 0;
}

