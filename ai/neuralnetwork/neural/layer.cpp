#include "ai/neuralnetwork/neural/layer.h"

const QString Layer::LAYER_PARAMETER_SIZE = "SIZE";
const QString Layer::LAYER_PARAMETER_TYPE = "TYPE";
const QString Layer::LAYER_PARAMETER_ACTIVATION = "ACTIVATION";

Layer::Layer(int id_layer, NeuralNetwork* net, QMap<QString, double> parameters)
{
    m_id_layer = id_layer;
    m_net = net;
    m_parameters = parameters;
    m_type = static_cast<LayerType>(static_cast<qint32>(parameters[LAYER_PARAMETER_TYPE]));
    m_activation = static_cast<Neuron::ActivationFunction>(static_cast<int>(m_parameters[LAYER_PARAMETER_ACTIVATION]));
	initLayer();
}

Layer::~Layer()
{
}

int Layer::getId() const
{
    return m_id_layer;
}

void Layer::initLayer()
{
    m_neurons.clear();

    if (m_type == LayerType::STANDARD || m_type == LayerType::INPUT)
	{
        m_parameters[LAYER_PARAMETER_SIZE] += 1; //bias for the next layer
        m_neurons.reserve(static_cast<int>(m_parameters[LAYER_PARAMETER_SIZE]));
        for (qint32 i_neuron = 0; i_neuron <m_parameters[LAYER_PARAMETER_SIZE]; ++i_neuron)
        {
            m_neurons.push_back(new Neuron(i_neuron, this, m_activation, i_neuron == m_neurons.capacity() - 1));
        }
	}
    else if (m_type == LayerType::OUTPUT)
	{
        m_neurons.reserve(static_cast<int>(m_parameters[LAYER_PARAMETER_SIZE]));
        for (qint32 i_neuron = 0; i_neuron < m_parameters[LAYER_PARAMETER_SIZE]; ++i_neuron)
        {
            m_neurons.push_back(new Neuron(i_neuron, this, m_activation));
        }
	}
}

void Layer::clean()
{
    for(auto & n : m_neurons)
    {
        n->clean();
    }
}

void Layer::trigger()
{
    for(auto & n : m_neurons)
    {
        n->trigger();
    }
}


void Layer::connectComplete(Layer *next)
{
    for(auto & n1 : m_neurons)
    {
        for(auto & n2 : next->m_neurons)
        {
			if(!n2->isBias())
            {
				n1->addNext(n2);
            }
        }
    }
}

QVector<double> Layer::output()
{
    QVector<double> outputs;
    outputs.reserve(m_neurons.size());
    for(auto & n : m_neurons)
    {
        outputs.push_back(n->output());
    }
    return outputs;

}

QVector<spNeuron> Layer::neurons() const
{
    return m_neurons;
}

void Layer::alterWeights(const QVector<QVector<double> >& weights)
{
    for(int i_neuron=0;i_neuron < weights.size(); ++i_neuron)
    {
        m_neurons[i_neuron]->alterWeights(weights[i_neuron]);
    }
}

void Layer::shiftBackWeights(const QVector<QVector<double> >& weights)
{
    for (int i_neuron = 0; i_neuron < m_neurons.size(); ++i_neuron)
    {
        m_neurons[i_neuron]->shiftBackWeights(weights[i_neuron]);
    }
}

QVector<QVector<double>> Layer::getWeights()
{
    QVector<QVector<double>> w;
    w.reserve(m_neurons.size());
    for (int i_neuron = 0; i_neuron < m_neurons.size(); ++i_neuron)
    {
        w.push_back(m_neurons[i_neuron]->getWeights());
    }
    return w;
}

QVector<QVector<spEdge> > Layer::getEdges()
{
    QVector<QVector<spEdge>> w;
    w.reserve(m_neurons.size());
    for (int i_neuron = 0; i_neuron < m_neurons.size(); ++i_neuron)
    {
        w.push_back(m_neurons[i_neuron]->getEdges());
    }
    return w;
}

void Layer::randomizeAllWeights(double abs_value)
{
    for(auto & neuron : m_neurons)
    {
        neuron->randomizeAllWeights(abs_value);
    }
}

QString Layer::toString()
{
    QString str = "layer: " + QString::number(m_id_layer) + "\n";
    for(auto & neuron : m_neurons)
    {
        str += neuron->toString() + "\n";
    }
    return str;
}

void Layer::shiftWeights(float range)
{
    for(auto & neuron : m_neurons)
    {
		neuron->shiftWeights(range);
    }
}

const QMap<QString, double>& Layer::getParameters() const
{
    return m_parameters;
}

QVector<QVector<double>> Layer::getBackpropagationShifts(const QVector<double>& target)
{
    QVector<QVector<double>> dw(m_neurons.size());
    for (qint32 i = 0; i < m_neurons.size(); i++)
	{
        spNeuron n = m_neurons[i];
		dw[i] = n->getBackpropagationShifts(target);
	}
	return dw;
}

Layer::LayerType Layer::getType() const
{
    return m_type;
}

Neuron::ActivationFunction Layer::getActivation() const
{
    return m_activation;
}
