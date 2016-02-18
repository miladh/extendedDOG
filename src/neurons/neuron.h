#ifndef NEURON_H
#define NEURON_H


#include "stimuli/stimuli.h"
#include "integrator.h"
#include "kernels/kernel.h"
#include "staticNonlinearity/staticnonlinearity.h"


using namespace arma;
using namespace std;
namespace lgnSimulator {
class Neuron
{
public:
    Neuron(const Integrator &integrator,
           const double backgroundResponse = 0,
           StaticNonlinearity *const staticNonlinearity = nullptr);
    ~Neuron();

    struct Input {
        Neuron* const neuron;
        const Kernel &kernel;
    };


    // Compute functions
    void computeResponse(Stimulus *stimulus);
    virtual void computeImpulseResponse();

    //Virtual function:
    virtual void computeImpulseResponseFourierTransform() = 0;


    // Add cell functions
    void addGanglionCell(Neuron *neuron, const Kernel &kernel);
    void addRelayCell(Neuron *neuron, const Kernel &kernell);
    void addInterNeuron(Neuron *neuron, const Kernel &kernel);
    void addCorticalNeuron(Neuron *neuron, const Kernel &kernel);


    // Getter member functions
    const cube &response() const;
    const cube &impulseResponse() const;
    const cx_cube &responseFT() const;
    const cx_cube &impulseResponseFourierTransform() const;

    vector<Input> ganglionCells() const;
    vector<Input> relayCells() const;
    vector<Input> interNeurons() const;
    vector<Input> corticalNeurons() const;

    string cellType() const;


    void clearResponse();
    void clearImpulseResponse();

    bool isImpulseResponseFourierTransformComputed() const;

private:
    const Integrator& m_integrator;
    StaticNonlinearity *const m_staticNonlinearity = nullptr;



protected:
    bool impulseResponseFourierTransformComputed  = false;
    const double m_backgroundResponse;
    string m_cellType;

    cube m_response;
    cube m_impulseResponse;
    cx_cube m_responseFT;
    cx_cube m_impulseResponseFT;

    vec m_spatialVec;
    vec m_spatialFreqs;

    vec m_timeVec;
    vec m_temporalFreqs;


    vector<Input> m_ganglionCells;
    vector<Input> m_relayCells;
    vector<Input> m_interNeurons;
    vector<Input> m_corticalNeurons;

};
}
#endif // NEURON_H
