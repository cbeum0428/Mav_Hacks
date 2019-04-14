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
		if(updateBoard(ANN_B, 0, inputs)) return;
		ANN_R.setCurrentInput(inputs);
		ANN_R.feedForward();
		if(updateBoard(ANN_R, 1, inputs)) return;
	}
}

bool updateBoard(ANN, int colorVal, inputs){
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
			
		if(inputs[(x_max*126)+(y_max*3)+2] and inputs[(x_max*126)+(y_max*3)+107];) break;// spot is full end
		Output[x_max*7+y_max] = 0;// spot full, highest ouput is now lowest
		ANN.setFitness(ANN.getFitness-4);
	}
	
	bool inverted = inputs[(x_max*7)+y_max+881]
	int z;
	if(inverted){// inverted = 1 place token going down
		for(z=0; z<6; z++){
			if(inputs[(x_max*126)+(z*21)+(y_max*3)+2]){
				inputs[(x_max*126)+(z*21)+(y_max*3)+2] = 0;
				inputs[(x_max*126)+(z*21)+(y_max*3)+colorVal] = 1;
			}
		}		
	}else{
		for(z=5; z>=0; z--){// inverted = 0 place token going up
			if(inputs[(x_max*126)+(z*21)+(y_max*3)+2]){
				inputs[(x_max*126)+(z*21)+(y_max*3)+2] = 0;
				inputs[(x_max*126)+(z*21)+(y_max*3)+colorVal] = 1;
			}
		}
	}
	//fitness update
	//check win condition
	double fittness=0;
	//+x
	int i;
	int numSelf=0;
	for(i=0; i<4 and inputs[((x_max+i)*126)+(z*21)+(y_max*3)+2] and (x_max+i < 7); ++i)//loop through blank spaces
	if (inputs[((x_max+i)*126)+(z*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max+i)*126)+(z*21)+(y_max*3) + colorVal] and (x_max+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max+i)*126)+(z*21)+(y_max*3) + Invert(colorVal)] and (x_max+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	//-x
	for(i=0; i<4 and inputs[((x_max-i)*126)+(z*21)+(y_max*3)+2] and (x_max-i >= 0); ++i)//loop through blank spaces
	if (inputs[((x_max-i)*126)+(z*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max-i)*126)+(z*21)+(y_max*3) + colorVal] and (x_max-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max-i)*126)+(z*21)+(y_max*3)+ Invert(colorVal)] and (x_max-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	//+y
	numSelf=0;
	for(i=0; i<4 and inputs[(x_max*126)+(z*21)+((y_max+i)*3)+2] and (y_max+i < 7); ++i)//loop through blank spaces
	if (inputs[(x_max*126)+(z*21)+((y_max+i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+(z*21)+((y_max+i)*3)+colorVal] and (y_max+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+(z*21)+((y_max+i)*3) + Invert(colorVal)] and (y_max+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	//-y
	for(i=0; i<4 and inputs[(x_max*126)+(z*21)+((y_max-i)*3)+2] and (x_max-i >= 0); ++i)//loop through blank spaces
	if (inputs[(x_max*126)+(z*21)+((y_max-i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+(z*21)+((y_max-i)*3) + colorVal] and (x_max-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+(z*21)+((y_max-i)*3) + Invert(colorVal)] and (x_max-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	//+z
	numSelf=0;
	for(i=0; i<4 and inputs[(x_max*126)+((z+i)*21)+(y_max*3)+2] and (z+i < 6); ++i)//loop through blank spaces
	if (inputs[(x_max*126)+((z+i)*21)+(y_max*3))*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+(y_max*3) + colorVal] and (z+i < 6); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+(y_max*3) Invert(colorVal)] and (z+i < 6); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	//-z
	for(i=0; i<4 and inputs[(x_max*126)+((z-i)*21)+(y_max*3)+2] and (z-i >= 0); ++i)//loop through blank spaces
	if (inputs[(x_max*126)+((z-i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+(y_max*3) + colorVal] and (z-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+(y_max*3) + Invert(colorVal)] and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	//+y+z
	numSelf=0;
	for(i=0; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max+i)*3)+2] and (y_max+i < 7) and (z+i < 7); ++i)//loop through blank spaces
	if (inputs[(x_max*126)+((z+i)*21)+((y_max+i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max+i)*3) + colorVal] and (y_max+i < 7) and (z+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max+i)*3) + Invert(colorVal)] and (y_max+i < 7) and (z+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}	
	//-y+z
	for(i=0; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max-i)*3)+2] and (x_max-i >= 0) and (z+i < 7); ++i)//loop through blank spaces
	if (inputs[(x_max*126)+((z+i)*21)+((y_max-i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max-i)*3) + colorVal] and (x_max-i >= 0) and (z+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max-i)*3) + Invert(colorVal)] and (x_max-i >= 0) and (z+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}	
	//+y-z
	numSelf=0;
	for(i=0; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max+i)*3)+2] and (y_max+i < 7) and (z-i >= 0); ++i)//loop through blank spaces
	if (inputs[(x_max*126)+((z-i)*21)+((y_max+i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max+i)*3) + colorVal] and (y_max+i < 7) and (z+i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max+i)*3) + Invert(colorVal)] and (y_max+i < 7) and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}	
	//-y-z
	for(i=0; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max-i)*3)+2] and (x_max-i >= 0) and (z-i >= 0); ++i)//loop through blank spaces
	if (inputs[(x_max*126)+((z-i)*21)+((y_max-i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max-i)*3) + colorVal] and (x_max-i >= 0) and (z-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max-i)*3) + Invert(colorVal)] and (x_max-i >= 0) and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}	
	//+x+z
	numSelf=0;
	for(i=0; i<4 and inputs[((x_max+i)*126)+((z+i)*21)+(y_max*3)+2] and (x_max+i < 7) and (z+i < 7); ++i)//loop through blank spaces
	if (inputs[((x_max+i)*126)+((z+i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max+i)*126)+((z+i)*21)+(y_max*3) + colorVal] and (x_max+i < 7) and (z+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max+i)*126)+((z+i)*21)+(y_max*3) + Invert(colorVal)] and (x_max+i < 7) and (z+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}	
	//-x+z
	for(i=0; i<4 and inputs[((x_max-i)*126)+(z+i)*21)+(y_max*3)+2] and (x_max-i >= 0) and (z+i < 7); ++i)//loop through blank spaces
	if (inputs[((x_max-i)*126)+((z+i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max-i)*126)+((z+i)*21)+(y_max*3) + colorVal] and (x_max-i >= 0) and (z+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max-i)*126)+((z+i)*21)+(y_max*3)+ Invert(colorVal)] and (x_max-i >= 0) and (z+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}	
	//+x-z
	numSelf=0;
	for(i=0; i<4 and inputs[((x_max+i)*126)+((z-i)*21)+(y_max*3)+2] and (x_max+i < 7) and (z-i >= 0); ++i)//loop through blank spaces
	if (inputs[((x_max+i)*126)+((z-i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max+i)*126)+((z-i)*21)+(y_max*3) + colorVal] and (x_max+i < 7) and (z-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max+i)*126)+((z-i)*21)+(y_max*3) + Invert(colorVal)] and (x_max+i < 7) and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}	
	//-x-z
	for(i=0; i<4 and inputs[((x_max-i)*126)+(z-i)*21)+(y_max*3)+2] and (x_max-i >= 0) and (z-i >= 0); ++i)//loop through blank spaces
	if (inputs[((x_max-i)*126)+((z-i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max-i)*126)+((z-i)*21)+(y_max*3) + colorVal] and (x_max-i >= 0) and (z-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN.setFitness(ANN.getFitness + fittness)
			return true;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max-i)*126)+((z-i)*21)+(y_max*3)+ Invert(colorVal)] and (x_max-i >= 0) and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	return false;	
}

int invert(int i){
	if(i) return 0;
	else return 1;
}
