#include <iostream>
#include "NeuralNetwork.h"
#include "Game.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

double topRedScore = 0;
double topBlackScore = 0;
int population;
double mr = 3;

void loadGameBoardState(int inversions[7][7], char board[7][6][7]) {
	string gamefn = "gameinfo.txt";
	ifstream gamefile(gamefn.c_str());
	string s;
	getline(gamefile, s);
	getline(gamefile, s); //Skip Inverts and {
	for (int x = 0;x < 7;x++) {
		getline(gamefile, s);
		int i = 0;
		while (s[i] != '{') {
			i++;
		}
		for (int y = 0;y < 7;y++) {
			inversions[x][y] = (int)s[++i] - '0';
		}
	}
	getline(gamefile, s);
	getline(gamefile, s);
	for (int x = 0;x < 7;x++) {
		for (int y = 0;y < 7;y++) {
			cout<<inversions[x][y]<<" ";
		}
		cout<<endl;
	}
	
	for (int x = 0;x < 7;x++) {
		getline(gamefile, s);
		getline(gamefile, s);
		for (int z = 0;z < 6;z++) {
			getline(gamefile, s);
			int i = 0;
			while (s[i] != '{') {
				i++;
			}
			i++;
			for (int y = 0;y < 7;y++) {
				board[x][z][y] = s[i++];
			}
		}
		getline(gamefile, s);
	}
	for (int x = 0;x < 7;x++) {
		cout<<"Game board x="<<x+1<<endl;
		for (int z = 0;z < 6;z++) {
			cout<<"Z: "<<z<<"| ";
			for (int y = 0;y < 7;y++) {
				cout<<board[x][z][y]<<", ";
			}
			cout<<" |";
			cout<<endl;
		}
		cout<<endl;
	}
}

string getUserInput(string prmpt) {
	cout<<prmpt;
	cin>>prmpt;
	return prmpt;
}

bool sortCompare(NeuralNetwork *a, NeuralNetwork *b) {
	return a->getFitness() > b->getFitness();
}

int main() {
	srand(time(NULL));
	bool isBlackPlayer = true;
	
	string mode = getUserInput("What mode are we in?: ");
	if (mode == "training") {
		string s = getUserInput("Population: ");
		population = atoi(s.c_str());
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
		cout<<"Created "<<redpop.size()<<" Red Neural Netorks"<<endl;
		
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
		cout<<"Created "<<blackpop.size()<<" Black Neural Netorks"<<endl;
		
		int result = 0;
		int topScore = 0;
		int highestScore = 0;
		for (int i=0; i<population;i++){//reset fitness values
			 blackpop[i]->setFitness(0);
			 redpop[i]->setFitness(0);
		 }
		for (int i = 0;i < numTrials;i++) {
			for (int j = 0;j < population;j++) {
				Game*g = new Game(blackpop[j], redpop[j]);// face black vs red
				result = g->pit_against();
				delete g;
			}
			if(topScore >= highestScore) highestScore = topScore;
			cout<<"Highest Score: "<<highestScore<<endl;
			sort(redpop.begin(), redpop.end(), sortCompare);
			sort(blackpop.begin(), blackpop.end(), sortCompare);
			
			for(int i=0; i< blackpop.size(); i++) blackpop[i]->setFitness(0);
			for(int i=0; i< redpop.size(); i++) redpop[i]->setFitness(0);
		}
		
		//breed nns.
		int numToKeep = population * 0.25;
		redpop.erase(redpop.begin() + numToKeep, redpop.end());
		blackpop.erase(blackpop.begin() + numToKeep, blackpop.end());
		for (int i = 0;i < numToKeep;i++) {
			redpop.push_back(redpop[i]->breed(mr));
			redpop.push_back(redpop[i]->breed(mr));
			redpop.push_back(redpop[i]->breed(mr));
			
			blackpop.push_back(blackpop[i]->breed(mr));
			blackpop.push_back(blackpop[i]->breed(mr));
			blackpop.push_back(blackpop[i]->breed(mr));
		}
		mr*=0.1;
		
		cout<<redpop.size()<<endl;
	} else if (mode == "tournament") {
		
	}
}
