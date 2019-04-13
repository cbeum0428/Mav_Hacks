#ifndef LAYER_H
#define LAYER_H
#include <iostream>
#include <vector>
#include "Neuron.h"
#include "Matrix.h"
using namespace std;
class Layer {
	public:
		Layer(int, string);
		void dumpLayer();
		void setValue(int, double);
		double getValue(int);
		string getName();
		Matrix *matrixifyValues();
		Matrix *matrixifyActivatedValues();
		Matrix *matrixifyDerivedValues();
		vector<Neuron *> getNeurons();
		
	private:
		string name;
		int size;
		vector<Neuron *> neurons;
};

Layer::Layer(int s, string n) {
	size = s;
	name = n;
	for (int i = 0;i < size;i++) {
		Neuron *n = new Neuron(0.00);
		neurons.push_back(n);
	}
}

void Layer::dumpLayer() {
	cout<<"[ ";
	for (int i = 0;i < size;i++) {
		cout<<(*neurons[i]).getValue();
		if (i < size - 1) {
			cout<<", ";
		}
	}
	cout<<" ]"<<endl;
}

void Layer::setValue(int i, double v) {
	neurons.at(i)->setValue(v);
}

double Layer::getValue(int i) {
	return neurons.at(i)->getValue();
}

Matrix *Layer::matrixifyValues() {
	Matrix *m = new Matrix(1, neurons.size(), false);
	for (int i = 0;i < neurons.size();i++) {
		m->setValue(0, i,  neurons.at(i)->getValue());
	}
	return m;
}

Matrix *Layer::matrixifyActivatedValues() {
	Matrix *m = new Matrix(1, neurons.size(), false);
	for (int i = 0;i < neurons.size();i++) {
		m->setValue(0, i,  neurons.at(i)->getActivatedValue());
	}
	return m;
}

Matrix *Layer::matrixifyDerivedValues() {
	Matrix *m = new Matrix(1, neurons.size(), false);
	for (int i = 0;i < neurons.size();i++) {
		m->setValue(0, i,  neurons.at(i)->getDerivedValue());
	}
	return m;
}

string Layer::getName() {
	return name;
}

vector<Neuron *> Layer::getNeurons() {
	//cout<<"Neurons: "<<neurons.size()<<endl;
	return neurons;
}
#endif
