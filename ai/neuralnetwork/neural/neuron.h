#ifndef NEURON_H
#define NEURON_H

#include <QVector>

#include "ai/neuralnetwork/neural/edge.h"
#include "oxygine-framework.h"

class Layer;

class Neuron;
using spNeuron = oxygine::intrusive_ptr<Neuron>;

class Neuron : public oxygine::ref_counter
{
public:

    enum class ActivationFunction
    {
        LINEAR,
        SIGMOID,
        RELU
    };

    Neuron(qint32 id_neuron, Layer* layer, ActivationFunction function = ActivationFunction::LINEAR, bool is_bias = false);

    ~Neuron();

    void trigger();

    double in();

    double output();

    double outputDerivative();

    double outputRaw();

    void clean();

    void addAccumulated(double v);

    void addNext(spNeuron n);

    void addPrevious(spEdge e);

    int getNeuronId() const;

    void setAccumulated(double v);

    void alterWeights(const QVector<double>& weights);

    QVector<double> getWeights();

    QVector<spEdge> getEdges();

    void randomizeAllWeights(double abs_value);

    QString toString();

    void shiftWeights(float range);

    void shiftBackWeights(const QVector<double>& range);

    QVector<double> getBackpropagationShifts(const QVector<double>& target);

    bool isBias() const;

    const Layer* getLayer() const
    {
        return m_layer;
    }
private:
    Layer* m_layer = nullptr;
    qint32 m_id_neuron = 0;
    double m_accumulated = 0.0;

    double m_threshold = 0.0;
    QVector<spEdge> m_next;
    QVector<spEdge> m_previous;
    ActivationFunction m_activation_function;
    bool m_is_bias = false;
};

#endif // NEURON_H
