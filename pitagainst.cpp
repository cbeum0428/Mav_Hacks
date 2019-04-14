#include <iostream>
#include <ctime>
#include <cstdlib>
#include "NeuralNetwork.h"
using namespace std;

void pit_against(ANN_B, ANN_R){
	// set fitness values to 0
	vector<double> inputs;
	for(int i=0; i<294; i++){
		inputs.pushback(0);
		inputs.pushback(0);
		inputs.pushback(1);
	}
	srand(time(NULL));
	for(int i=0; i<49; i++){// add on random inverted spots
		inputs.pushback(double(rand() % 2));
	}
	for(int i=0; i<294; i++){// iterate for as much spaces in the board
		ANN_B.setCurrentInput(inputs);
		ANN_B.feedForward();
		updateBoard(ANN_B, inputs);
		ANN_R.setCurrentInput(inputs);
		ANN_R.feedForward();
		updateBoard(ANN_R, inputs);
	}
}

void updateBoard(ANN, inputs){
	vector<double> Output = ANN.getOutputValues;
	int x_max = 0;
	int y_max = 0;
	for(int j=0; j<49; j++){
		int val_max = 0;
		for(int i=0; i<49; i++){ // get x y coords of highest output
			if(Output[i]>=val_max){
				x_max = i/7;
				y_max = i-x_max*7;
				val_max = Output[i];
			}
		}
			
		if(inputs[(x_max*126)+(y_max*3)+2] and inputs[(x_max*126)+(y_max*3)+128];) break;// spot is full end
		Output[x_max*7+y_max] = 0;// spot full, highest ouput is now lowest
		//fitness update
	}
	//fitness update
	//check win condition
	bool inverted = inputs[(x_max*7)+y_max+881]
}
