#include <iostream>
#include "NeuralNetwork.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <stdlib.h>

using namespace std;

string getUserInput(string prmpt) {
	cout<<prmpt;
	cin>>prmpt;
	return prmpt;
}

int main() {
	
	vector<int> topology;
	topology.push_back(3);
	topology.push_back(2);
	
	vector<NeuralNetwork *> networks;
	NeuralNetwork *n1 = new NeuralNetwork(topology);
	n1->setFitness(100);
	NeuralNetwork *n2 = new NeuralNetwork(topology);
	n2->setFitness(0);
	
	networks.push_back(n1);
	networks.push_back(n2);
	
	//sort(networks);
	
	for (int i = 0;i < networks.size();i++) {
		cout<<i<<": "<<networks[i]->getFitness()<<endl;
	}
	
	/*
	srand(time(NULL));
	bool isBlackPlayer = true;
	
	string mode = getUserInput("What mode are we in?: ");
	if (mode == "training") {
		string s = getUserInput("Population: ");
		int population = atoi(s.c_str());
		s = getUserInput("Number of pit against trials?: ");
		int numTrials = atoi(s.c_str());
		
		
		vector<NeuralNetwork *> redpop;
		for (int i = 0;i < population;i++) {
			
			//Generate random Hidden Layer information
			int numHLayers = rand() % 5;
			
			//Build a random topology
			vector<int> topology;
			topology.push_back(931);
			for (int j = 0;j < numHLayers;j++) {
				int numHNeurons = rand() % 1000;
				topology.push_back(numHNeurons);
			}
			topology.push_back(49);
			
			NeuralNetwork *nn = new NeuralNetwork(topology);
			redpop.push_back(nn);
			cout<<i<<": ";
			redpop[i]->printTopology();
		}
		cout<<"Created "<<redpop.size()<<"Red Neural Netorks"<<endl;
		
		vector<NeuralNetwork *> blackpop;
		for (int i = 0;i < population;i++) {
			
			//Generate random Hidden Layer information
			int numHLayers = rand() % 5;
			
			//Build a random topology
			vector<int> topology;
			topology.push_back(931);
			for (int j = 0;j < numHLayers;j++) {
				int numHNeurons = rand() % 1000;
				topology.push_back(numHNeurons);
			}
			topology.push_back(49);
			
			NeuralNetwork *nn = new NeuralNetwork(topology);
			blackpop.push_back(nn);
			cout<<i<<": ";
			blackpop[i]->printTopology();
		}
		cout<<"Created "<<blackpop.size()<<"Black Neural Netorks"<<endl;
		
		for (int i = 0;i < numTrials;i++) {
			for (int j = 0;j < population;j++) {
				//play a game against 
			}
		}
	} else if (mode == "tournament") {
		
	}
	*/

}
