#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include <iostream>
#include <vector>
#include <algorithm>
#include "Matrix.h"
#include "Layer.h"
#include "MultiplyMatrix.h"
using namespace std;
class NeuralNetwork {
	public:
		//Constructors
		NeuralNetwork(vector<int>);
		
		//Void Functions
		void setCurrentInput(vector<double>);
		void feedForward();
		void printToConsole();
		void setNeuronValue(int, int, double);
		void setErrors();
		void setCurrentTarget(vector<double>);
		void backPropigation();
		
		//Matrix Functions
		Matrix *getNeuronMatrix(int);
		Matrix *getActivatedNeuronMatrix(int);
		Matrix *getDerivedNeuronMatrix(int);
		Matrix *getWeightMatrix(int);
		
		vector<double> getOutputValues();
		
		double getTotalError();
		vector<double> getErrors();
		void setFitness(double);
		double getFitness();
		
		void setWeightMatricies(vector<Matrix *>);
		vector<Matrix *> getWeightMatricies();
		void printTopology();
	private:
		int topologySize; 
		vector<int> topology;
		vector<Layer *> layers;
		vector<Matrix *> weightMatricies;
		vector<double> input;
		vector<double> target;
		double error;
		vector<double> errors;
		vector<double> historicalErrors;
		double fitness;
};

NeuralNetwork::NeuralNetwork(vector<int> t) {
	fitness = 0;
	topology = t;
	topologySize = t.size();
	for (int i = 0;i < (topologySize);i++) {
		string name ="";
		if (i == 0) {
			name = "Input Layer";
		} else if (i == topologySize - 1) {
			name = "Output Layer";
		} else {
			name = "Hidden Layer " + to_string(i);
		}
		
		Layer *l = new Layer(topology.at(i), name);
		layers.push_back(l);
	}
	
	for (int i = 0;i < (topologySize - 1);i++) {
		Matrix *m = new Matrix(topology.at(i), topology.at(i + 1), true);
		weightMatricies.push_back(m);
	}
}

void NeuralNetwork::setCurrentInput(vector<double> in) {
	input = in;
	for (int i = 0;i < in.size();i++) {
		layers.at(0)->setValue(i, input.at(i));
	}
}

void NeuralNetwork::printToConsole() {
	for (int i = 0;i < layers.size();i++) {
		cout<<layers.at(i)->getName()<<": "<<endl;
		if (i == 0) {
			Matrix *m = layers.at(i)->matrixifyValues();
			m->printToConsole();
		} else {
			Matrix *m = layers.at(i)->matrixifyActivatedValues();
			m->printToConsole();
		}		
	}
}

void NeuralNetwork::feedForward() {
	for (int i = 0;i < (layers.size() - 1);i++) {
		Matrix *a = getNeuronMatrix(i);
		
		if (i != 0) {
			a = getActivatedNeuronMatrix(i);
		}
		
		Matrix *b = getWeightMatrix(i);
		MultiplyMatrix *m = new MultiplyMatrix(a, b);
		Matrix *c = m->execute();
		
		for (int ci = 0;ci < c->getNumCols();ci++) {
			setNeuronValue(i + 1, ci, c->getValue(0, ci));
		}
	}
}

void NeuralNetwork::setNeuronValue(int indexLayer, int indexNeuron, double value) {
	layers.at(indexLayer)->setValue(indexNeuron, value);
	
}

Matrix *NeuralNetwork::getNeuronMatrix(int index) {
	return layers.at(index)->matrixifyValues();
}
Matrix *NeuralNetwork::getActivatedNeuronMatrix(int index) {
	return layers.at(index)->matrixifyActivatedValues();
}
Matrix *NeuralNetwork::getDerivedNeuronMatrix(int index) {
	return layers.at(index)->matrixifyDerivedValues();
}
Matrix *NeuralNetwork::getWeightMatrix(int index) {
	return weightMatricies.at(index);
}

//Sum up errors
double NeuralNetwork:: getTotalError() {
	return error;
}

vector<double> NeuralNetwork::getErrors() {
	return errors;
}

void NeuralNetwork::setErrors() {
	if (target.size() == 0) {
		cout<<"No target..."<<endl;
		exit(0);
	}
	
	if (target.size() != layers.at(layers.size() - 1)->getNeurons().size()) {
		cout<<"Target size is not the same as the output layer size"<<endl;
		exit(0);
	}
	
	error = 0;
	int outputLayerIndex = layers.size() - 1;
	//cout<<"output index: "<<outputLayerIndex<<endl;
	vector<Neuron *> outputNeurons = layers.at(outputLayerIndex)->getNeurons();
	for (int i = 0;i < target.size();i++) {
		double tempErr = (outputNeurons.at(i)->getActivatedValue() - target.at(i));
		errors.at(i) = tempErr;
		error += tempErr;
	}
	
	historicalErrors.push_back(error);
}

void NeuralNetwork::setCurrentTarget(vector<double> t) {
	target = t;
	errors.clear();
	for (int i = 0;i < target.size();i++) {
		errors.push_back(0);
	}
}

void NeuralNetwork::backPropigation() {
	vector<Matrix *> newWeights;
	Matrix * gradient;
	int outputLayerIndex = (layers.size() - 1);
	Matrix *derivedValuesYtoZ = layers.at(outputLayerIndex)->matrixifyDerivedValues();
	Matrix *gradientsYtoZ = new Matrix(1, layers.at(outputLayerIndex)->getNeurons().size(), false);
	for (int i = 0;i < errors.size();i++) {
		double d = derivedValuesYtoZ->getValue(0, i);
		double e = errors.at(i);
		double g = d * e;
		gradientsYtoZ->setValue(0, i, g);
	}
	
	int lastHiddenLayerIndex = (outputLayerIndex - 1);
	Layer *lastHiddenLayer = layers.at(lastHiddenLayerIndex);
	Matrix *weightsOutputToHidden = weightMatricies.at(outputLayerIndex - 1);
	MultiplyMatrix *m = new MultiplyMatrix(gradientsYtoZ->transpose(), lastHiddenLayer->matrixifyActivatedValues());
	Matrix *deltaOutputToHidden = m->execute()->transpose();
	
	
	Matrix *newWeightsOutputToHidden = new Matrix(	deltaOutputToHidden->getNumRows(), 
													deltaOutputToHidden->getNumCols(), 
													false);
	for (int r = 0;r < deltaOutputToHidden->getNumRows();r++) {
		for (int c = 0;c < deltaOutputToHidden->getNumCols();c++) {
			double originalWeight = weightsOutputToHidden->getValue(r, c);
			double deltaWeight = deltaOutputToHidden->getValue(r, c);
			newWeightsOutputToHidden->setValue(r, c, originalWeight - deltaWeight);
		}
	}
	
	newWeights.push_back(newWeightsOutputToHidden);
	gradient = new Matrix(gradientsYtoZ->getNumRows(), gradientsYtoZ->getNumCols(), false);
	
	for (int r = 0;r < gradientsYtoZ->getNumRows();r++) {
		for (int c = 0;c < gradientsYtoZ->getNumCols();c++) {
			gradient->setValue(r, c, gradientsYtoZ->getValue(r, c));
		}
	}
	
	//adjust weights
	for(int i = lastHiddenLayerIndex;i > 0;i--) {
		Layer *l = layers.at(i);
		Matrix *derivedHidden = l->matrixifyDerivedValues();
		Matrix *activatedHidden = l->matrixifyActivatedValues();
		Matrix *derivedGradients = new Matrix(1, l->getNeurons().size(), false);
		Matrix *weightMatrix = weightMatricies.at(i);
		Matrix * originalWeight = weightMatricies.at(i - 1);
		
		
		for (int r = 0;r < weightMatrix->getNumRows();r++) {
			double sum = 0;
			for (int c = 0;c < weightMatrix->getNumCols();c++) {
				double p =  gradient->getValue(0, c) * weightMatrix->getValue(r, c);
				sum += p;
			}
			double g = sum * activatedHidden->getValue(0, r);
			derivedGradients->setValue(0, r, g);
		}
		
		Matrix *leftNeurons;
		if (i - 1 == 0) {
			leftNeurons = layers.at(0)->matrixifyValues();
		} else {
			leftNeurons = layers.at(i - 1)->matrixifyActivatedValues();
		}
		
		MultiplyMatrix *mul = new MultiplyMatrix(derivedGradients->transpose(), leftNeurons);
		Matrix *deltaWeights = mul->execute()->transpose();
		
		Matrix *newWeightsHidden = new Matrix(deltaWeights->getNumRows(), deltaWeights->getNumCols(), false);
		for (int r = 0;r < newWeightsHidden->getNumRows();r++) {
			for (int c = 0;c < newWeightsHidden->getNumCols();c++) {
				double w = originalWeight->getValue(r, c);
				double d = deltaWeights->getValue(r, c);
				double n = w - d;
				newWeightsHidden->setValue(r, c, n);
			}
		}
		
		
		gradient = new Matrix(derivedGradients->getNumRows(), derivedGradients->getNumCols(), false);
	
		for (int r = 0;r < derivedGradients->getNumRows();r++) {
			for (int c = 0;c < derivedGradients->getNumCols();c++) {
				gradient->setValue(r, c, derivedGradients->getValue(r, c));
			}
		}
		
		newWeights.push_back(newWeightsHidden);
	}
	reverse(newWeights.begin(), newWeights.end());
	weightMatricies = newWeights;
}

vector<double> NeuralNetwork::getOutputValues() {
	vector<double> outs;
	vector<Neuron *> outputLayer = layers.at(layers.size() - 1)->getNeurons();
	
	for (int i = 0;i < outputLayer.size();i++) {
		outs.push_back(outputLayer.at(i)->getValue());
	}
	
	return outs;
}

void NeuralNetwork::setFitness(double f) {
	fitness = f;
}

double NeuralNetwork::getFitness() {
	return fitness;
}

void NeuralNetwork::setWeightMatricies(vector<Matrix *> newWeights) {
	weightMatricies = newWeights;
}

vector<Matrix *> NeuralNetwork::getWeightMatricies() {
	return weightMatricies;
}

void NeuralNetwork::printTopology() {
	for (int i = 0;i < topology.size();i++) {
		cout<<topology[i]<<"->";
	}
	cout<<endl;
}
#endif
