#ifndef NEURON_H
#define NEURON_H
#include <iostream>
#include <cmath>
using namespace std;
class Neuron {
	public:
		//Constructor
		Neuron();
		Neuron(double);
		//Fast sigmoid function
		//f(x) = x / (1 + abs(x));
		void activate();
		//Derivative of fast sigmoid
		//f'(x) = f(x) * (1 - (f(x))
		void derive();
		//Getters
		void setValue(double);
		double getValue();
		double getActivatedValue();
		double getDerivedValue();
	private:
		//Value input.
		double value;
		//Activation value. 
		//Squished to fit in the range 0-1.
		double activatedValue;
		//Approximated derivative of activated value.
		double derivedValue; 
};

Neuron::Neuron() {
	
}

Neuron::Neuron(double val) {
	value = val;
	activate();
	derive();
}

void Neuron::activate() {
	activatedValue = value / (1.0 + abs(value));
}

void Neuron::derive() {
	derivedValue = activatedValue * (1.0 - activatedValue);
}

void Neuron::setValue(double val) {
	value = val;
	activate();
	derive();
}

double Neuron::getValue() {
	return value;
}

double Neuron::getActivatedValue() {
	return activatedValue;
}

double Neuron::getDerivedValue() {
	return derivedValue;
}
#endif
