#include <iostream>
#include "NeuralNetwork.h"
#include "Game.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <unistd.h>

using namespace std;

double topRedScore = 0;
double topBlackScore = 0;
int population;
double mr = 3;
string mode;

string getFileContents(string fn) {
	ifstream infile(fn.c_str());
	string fullFile = "";
	string s;
	getline(infile, s);
	while (infile) {
		fullFile += s;
		getline(infile, s);
	}
	
	return fullFile;
}

bool sortCompare(NeuralNetwork *a, NeuralNetwork *b) {
	return a->getFitness() > b->getFitness();
}

void saveTraining(vector<NeuralNetwork*> blackpop, vector<NeuralNetwork*> redpop) {
	sort(redpop.begin(), redpop.end(), sortCompare);
	sort(blackpop.begin(), blackpop.end(), sortCompare);
	
	for (int i = 0;i < population;i++) {
		stringstream rss;
		stringstream bss;
		rss.clear();
		bss.clear();
		rss<<"Training/Red/"<<i<<".txt";
		bss<<"Training/Black/"<<i<<".txt";
		redpop[i]->outputToFile(rss.str());
		blackpop[i]->outputToFile(bss.str());
	}
}

void loadGameBoardState(int inversions[7][7], char board[7][6][7]) {
	string gamefn = "gameinfo.txt";
	ifstream gamefile(gamefn.c_str());
	string s;
	getline(gamefile, s);
	getline(gamefile, s);
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
			//cout<<inversions[x][y]<<" ";
		}
		//cout<<endl;
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
		//cout<<"Game board x="<<x+1<<endl;
		for (int z = 0;z < 6;z++) {
			//cout<<"Z: "<<z<<"| ";
			for (int y = 0;y < 7;y++) {
				//cout<<board[x][z][y]<<", ";
			}
			//cout<<" |";
			//cout<<endl;
		}
		//cout<<endl;
	}
}

string getUserInput(string prmpt) {
	cout<<prmpt;
	cin>>prmpt;
	return prmpt;
}

int main() {
	srand(time(NULL));
	//loadTopScores();
	mode = "tournament";
	//mode = getUserInput("What mode are we in?: ");
	if (mode == "training") {
		string s = getUserInput("Population: ");
		population = atoi(s.c_str());
		s = getUserInput("Number of pit against trials?: ");
		int numTrials = atoi(s.c_str());
		s = getUserInput("Enter number of Epochs");
		int numEpochs = atoi(s.c_str());
		
		
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
		for (int n = 0;n < numEpochs; n++) {
			cout<<"Epoch: "<<n<<"/"<<numEpochs<<endl;
			for (int i = 0;i < numTrials;i++) {
				for (int j = 0;j < population;j++) {
					//play game.
				}
				sort(redpop.begin(), redpop.end(), sortCompare);
				sort(blackpop.begin(), blackpop.end(), sortCompare);
				
				if (redpop[0]->getFitness() > topRedScore) {
					topRedScore = redpop[0]->getFitness();
				}
				
				if (blackpop[0]->getFitness() > topBlackScore) {
					topBlackScore = blackpop[0]->getFitness();
				}
		
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
		}
		sort(blackpop.begin(), blackpop.end(), sortCompare);
		sort(redpop.begin(), redpop.end(), sortCompare);
		
		blackpop[0]->outputToFile("DeanKelleyB.txt");
		redpop[0]->outputToFile("DeanKelleyR.txt");
		//saveTraining(blackpop, redpop);
	} else if (mode == "tournament") {
		bool isBlackPlayer;
		bool isFirstTurn = true;
		string s = getUserInput("What player am i?: (b for black, r for red)");
		string fn;
		if (s == "b") {
			isBlackPlayer = true;
			fn = "DeanKelleyB.txt";
		} else {
			isBlackPlayer = false;
			fn = "DeanKelleyR.txt";
		}
		
		vector<int> top;
		top.push_back(931);
		top.push_back(767);
		top.push_back(49);
		
		NeuralNetwork* nn = new NeuralNetwork(top);
		
		//NeuralNetwork* nn = new NeuralNetwork(fn);
		
		bool isMyTurn = false;
		if (isBlackPlayer) {
			isMyTurn = true;
		}
		
		string gamefn = "gameinfo.txt";
		string lastUpdateFile = "";
		while (1) {
			usleep(100000);
			string newFileContents = getFileContents(gamefn);
			if ((isBlackPlayer and isFirstTurn) or (newFileContents != lastUpdateFile and isMyTurn)) {
				if (isFirstTurn) {
					isFirstTurn = false;
				}
				cout<<"My turn taking a turn"<<endl;
				int inversions[7][7];
				char board[7][6][7];
				loadGameBoardState(inversions, board);
				
				//Load x,Z,y
				vector<double> inputs;
				for (int x = 0;x < 7;x++) {
					for (int z = 0;z < 6;z++) {
						for (int y = 0;y < 7;y++) {
							if (board[x][z][y] == 'B') {
								inputs.push_back(1);
								inputs.push_back(0);
								inputs.push_back(0);
							} else if (board[x][z][y] == 'R') {
								inputs.push_back(0);
								inputs.push_back(1);
								inputs.push_back(0);
							} else {
								inputs.push_back(0);
								inputs.push_back(0);
								inputs.push_back(1);
							}
						}
					}
				}
				
				for (int x = 0;x < 7;x++) {
					for (int y = 0;y < 7;y++) {
						inputs.push_back(inversions[x][y]);
					}
				}
				nn->setCurrentInput(inputs);
				nn->feedForward();
				
				vector<double> output = nn->getOutputValues();
				
				double maxVal = output[0];
				int maxR = 0;
				int maxC = 0;
				int maxIndex = 0;
				int r = 0;
				int c = 0;
				for (int i = 1;i < 49;i++) {
					c++;
					if (c >= 7) {
						cout<<endl<<output[i];
						r++;
						c = 0;
					} else {
						cout<<output[i]<<" ";
					}
					if (output[i] > maxVal) {
						maxVal = output[i];
						maxR = r;
						maxC = c;
						maxIndex = i;
					}
				}
				
				cout<<endl;
				
				int x = maxC;
				int y = maxR;
				int z = 0;
				if (inversions[maxC][maxR] == 1) {
					z = 5;
				}
				
				while(board[x][z][y] == 'B' or board[x][z][y] == 'R') {
					output[maxIndex] = 0;
					for (int i = 1;i < 49;i++) {
						c++;
						if (c >= 7) {
							r++;
							c = 0;
						}
						if (output[i] > maxVal) {
							maxVal = output[i];
							maxR = r;
							maxC = c;
							maxIndex = 0;
						}
					}
				}
				cout<<maxVal<<endl;
				int guessX = maxC+1;
				int guessY = maxR+1;
				
				cout<<guessY<<" "<<guessX<<endl;
				isMyTurn = false;
				lastUpdateFile = newFileContents;
			} else if (newFileContents != lastUpdateFile and !isMyTurn) {
				isMyTurn = true;
				lastUpdateFile = newFileContents;
				if (isFirstTurn) {
					isFirstTurn = false;
				}
			}
		}
	}
}
