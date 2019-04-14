#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "NeuralNetwork.h"
using namespace std;

class Game{
	public:
		Game(NeuralNetwork *, NeuralNetwork *);
		int pit_against();
	private:
		NeuralNetwork * ANN_B, *ANN_R;
		bool updateBoard(NeuralNetwork *, int, vector<double>&);
		int Invert(int);
		
};

Game::Game(NeuralNetwork* b, NeuralNetwork* r) {
	ANN_B = b;
	ANN_R = r;
}

int Game::pit_against(){
	//int result;
	// set fitness values to 0
	vector<double> inputs;
	for(int i=0; i<294; i++){
		inputs.push_back(0);
		inputs.push_back(0);
		inputs.push_back(1);
	}
	srand(time(NULL));
	for(int i=0; i<49; i++){// add on random inverted spots
		inputs.push_back(double(rand() % 2));
	}
	for(int i=0; i<294; i++){// iterate for as much spaces in the board
		/*
		for(int x=0; x<7; x++){
			cout<<"x="<<x+1<<endl;
			for(int z=0; z<6; z++){
				cout<<"{";
				for(int y = 0; y < 7; y++){
					if(inputs[(x*126)+(z*21)+(y*3)+0]) cout<<"R";
					else if(inputs[(x*126)+(z*21)+(y*3)+1]) cout<<"B";
					else cout<<"_";
				}
				cout<<"}"<<endl;
			}
		}
		string pause;
		cin>>pause;
		*/
		
		ANN_B->setCurrentInput(inputs);
		ANN_B->feedForward();
		//result = updateBoard(ANN_B, 0, inputs);
		if(updateBoard(ANN_B, 0, inputs)){
			/*
			cout<<"Black Wins! "<<endl;;
			for(int x=0; x<7; x++){
				cout<<"x="<<x+1<<endl;
				for(int z=0; z<6; z++){
					cout<<"{";
					for(int y = 0; y < 7; y++){
						if(inputs[(x*126)+(z*21)+(y*3)+0]) cout<<"R";
						else if(inputs[(x*126)+(z*21)+(y*3)+1]) cout<<"B";
						else cout<<"|";
					}
					cout<<"}"<<endl;
				}
			}
			string pause;
			cin>>pause;
			* */
			ANN_B->setFitness(ANN_B->getFitness() + 294-i);
			return 2;
		}
		ANN_R->setCurrentInput(inputs);
		ANN_R->feedForward();
		//result = updateBoard(ANN_B, 0, inputs);
		if(updateBoard(ANN_R, 1, inputs)){
			/*
			cout<<"Red Wins! "<<endl;;
			for(int x=0; x<7; x++){
				cout<<"x="<<x+1<<endl;
				for(int z=0; z<6; z++){
					cout<<"{";
					for(int y = 0; y < 7; y++){
						if(inputs[(x*126)+(z*21)+(y*3)+0]) cout<<"B";
						else if(inputs[(x*126)+(z*21)+(y*3)+1]) cout<<"R";
						else cout<<"|";
					}
					cout<<"}"<<endl;
				}
			}
			string pause;
			cin>>pause;
			 */
			ANN_R->setFitness(ANN_R->getFitness() + 294-i);
			return 1;
		};
		//cout<<"iteration: "<<i<<endl;
	}
	return 0;
}

bool Game::updateBoard(NeuralNetwork * ANN, int colorVal, vector<double> &inputs){
	//string pause;
	//cin>>pause;
	vector<double> Output = ANN->getOutputValues();
	int x_max = 0;
	int y_max = 0;
	for(int j=0; j<49; j++){
		int val_max = 0;
		int valMax_i;
		for(int i=0; i<49; i++){ // get x y coords of highest output
			if(Output[i]>=val_max){
				//cout<<Output[i]<<endl;
				x_max = i/7;
				y_max = i-x_max*7;
				val_max = Output[i];
				valMax_i = i;
			}
		}
		//cout<<inputs[(x_max*126)+(y_max*3)+2]<<","<<inputs[(x_max*126)+(y_max*3)+107]<<endl;
		if((inputs[(x_max*126)+(y_max*3)+2] + inputs[(x_max*126)+(y_max*3)+107]) == 2){
			//cout<<"found one!"<<endl; 
			break;// spot is full end
		}
		ANN->setFitness(ANN->getFitness()-4);
		//cout<<x_max<<","<<y_max<<" Taken"<<endl;
		Output[valMax_i] = 0;
	}
	
	bool inverted = inputs[(x_max*7)+y_max+881];
	int z;
	if(inverted){// inverted = 1 place token going down
		for(z=0; z<6 and inputs[(x_max*126)+(z*21)+(y_max*3)+2] != 0;z++);
		z--;
		inputs[(x_max*126)+(z*21)+(y_max*3)+2] = 0;
		inputs[(x_max*126)+(z*21)+(y_max*3)+colorVal] = 1;		
	}else{
		for(z=5; z>=0 and inputs[(x_max*126)+(z*21)+(y_max*3)+2] != 0; z--);// inverted = 0 place token going up
		z++;
		inputs[(x_max*126)+(z*21)+(y_max*3)+2] = 0;
		inputs[(x_max*126)+(z*21)+(y_max*3)+colorVal] = 1;
	}
	//fitness update
	//check win condition
	double fittness=0;
	//cout<<"Color Val: "<<colorVal<<endl;
	//cout<<x_max<<","<<z<<","<<y_max<<endl;
	/*
	//+x
	int i;
	int numSelf=0;
	for(i=0; i<4 and inputs[((x_max+i)*126)+(z*21)+(y_max*3)+2] and (x_max+i < 7); ++i);//loop through blank spaces
	if (inputs[((x_max+i)*126)+(z*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max+i)*126)+(z*21)+(y_max*3) + colorVal] and (x_max+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 1;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max+i)*126)+(z*21)+(y_max*3) + Invert(colorVal)] and (x_max+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"+x"<<endl;
	//-x
	for(i=0; i<4 and (x_max-i >= 0) and inputs[((x_max-i)*126)+(z*21)+(y_max*3)+2]; ++i){
		};//loop through blank spaces
		//cout<<i<<endl;
	if (inputs[((x_max-i)*126)+(z*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max-i)*126)+(z*21)+(y_max*3) + colorVal] and (x_max-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 2;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max-i)*126)+(z*21)+(y_max*3)+ Invert(colorVal)] and (x_max-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"-x"<<endl;
	//+y
	numSelf=0;
	for(i=0; i<4 and inputs[(x_max*126)+(z*21)+((y_max+i)*3)+2] and (y_max+i < 7); ++i);//loop through blank spaces
	if (inputs[(x_max*126)+(z*21)+((y_max+i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+(z*21)+((y_max+i)*3)+colorVal] and (y_max+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 3;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+(z*21)+((y_max+i)*3) + Invert(colorVal)] and (y_max+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"+y"<<endl;
	//-y
	for(i=0; i<4 and inputs[(x_max*126)+(z*21)+((y_max-i)*3)+2] and (x_max-i >= 0); ++i);//loop through blank spaces
	if (inputs[(x_max*126)+(z*21)+((y_max-i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+(z*21)+((y_max-i)*3) + colorVal] and (x_max-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 4;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+(z*21)+((y_max-i)*3) + Invert(colorVal)] and (x_max-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"-y"<<endl;
	//+z
	numSelf=0;
	for(i=0; i<4 and inputs[(x_max*126)+((z+i)*21)+(y_max*3)+2] and (z+i < 6); ++i);//loop through blank spaces
	if (inputs[(x_max*126)+((z+i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+(y_max*3) + colorVal] and (z+i < 6); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 5;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+(y_max*3) + Invert(colorVal)] and (z+i < 6); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"+z"<<endl;
	//-z
	for(i=0; i<4 and inputs[(x_max*126)+((z-i)*21)+(y_max*3)+2] and (z-i >= 0); ++i);//loop through blank spaces
	if (inputs[(x_max*126)+((z-i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+(y_max*3) + colorVal] and (z-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 6;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+(y_max*3) + Invert(colorVal)] and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"-z"<<endl;
	//+y+z
	numSelf=0;
	for(i=0; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max+i)*3)+2] and (y_max+i < 7) and (z+i < 7); ++i);//loop through blank spaces
	if (inputs[(x_max*126)+((z+i)*21)+((y_max+i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max+i)*3) + colorVal] and (y_max+i < 7) and (z+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 7;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max+i)*3) + Invert(colorVal)] and (y_max+i < 7) and (z+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"+y+z"<<endl;	
	//-y+z
	for(i=0; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max-i)*3)+2] and (x_max-i >= 0) and (z+i < 7); ++i);//loop through blank spaces
	if (inputs[(x_max*126)+((z+i)*21)+((y_max-i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max-i)*3) + colorVal] and (x_max-i >= 0) and (z+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 8;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z+i)*21)+((y_max-i)*3) + Invert(colorVal)] and (x_max-i >= 0) and (z+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"-y+z"<<endl;		
	//+y-z
	numSelf=0;
	for(i=0; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max+i)*3)+2] and (y_max+i < 7) and (z-i >= 0); ++i);//loop through blank spaces
	if (inputs[(x_max*126)+((z-i)*21)+((y_max+i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max+i)*3) + colorVal] and (y_max+i < 7) and (z+i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 9;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max+i)*3) + Invert(colorVal)] and (y_max+i < 7) and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"+y-z"<<endl;		
	//-y-z
	for(i=0; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max-i)*3)+2] and (x_max-i >= 0) and (z-i >= 0); ++i);//loop through blank spaces
	if (inputs[(x_max*126)+((z-i)*21)+((y_max-i)*3)+colorVal]){
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max-i)*3) + colorVal] and (x_max-i >= 0) and (z-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 10;//game won!	
		} 
	}else{
		for(; i<4 and inputs[(x_max*126)+((z-i)*21)+((y_max-i)*3) + Invert(colorVal)] and (x_max-i >= 0) and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	cout<<"-y-z"<<endl;		
	//+x+z
	numSelf=0;
	for(i=0; i<4 and inputs[((x_max+i)*126)+((z+i)*21)+(y_max*3)+2] and (x_max+i < 7) and (z+i < 7); ++i);//loop through blank spaces
	if (inputs[((x_max+i)*126)+((z+i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max+i)*126)+((z+i)*21)+(y_max*3) + colorVal] and (x_max+i < 7) and (z+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 11;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max+i)*126)+((z+i)*21)+(y_max*3) + Invert(colorVal)] and (x_max+i < 7) and (z+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}	
	//-x+z
	for(i=0; i<4 and inputs[((x_max-i)*126)+((z+i)*21)+(y_max*3)+2] and (x_max-i >= 0) and (z+i < 7); ++i);//loop through blank spaces
	if (inputs[((x_max-i)*126)+((z+i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max-i)*126)+((z+i)*21)+(y_max*3) + colorVal] and (x_max-i >= 0) and (z+i < 7); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 12;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max-i)*126)+((z+i)*21)+(y_max*3)+ Invert(colorVal)] and (x_max-i >= 0) and (z+i < 7); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}	
	//+x-z
	numSelf=0;
	for(i=0; i<4 and inputs[((x_max+i)*126)+((z-i)*21)+(y_max*3)+2] and (x_max+i < 7) and (z-i >= 0); ++i);//loop through blank spaces
	if (inputs[((x_max+i)*126)+((z-i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max+i)*126)+((z-i)*21)+(y_max*3) + colorVal] and (x_max+i < 7) and (z-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 13;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max+i)*126)+((z-i)*21)+(y_max*3) + Invert(colorVal)] and (x_max+i < 7) and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}	
	//-x-z
	for(i=0; i<4 and inputs[((x_max-i)*126)+((z-i)*21)+(y_max*3)+2] and (x_max-i >= 0) and (z-i >= 0); ++i);//loop through blank spaces
	if (inputs[((x_max-i)*126)+((z-i)*21)+(y_max*3)+colorVal]){
		for(; i<4 and inputs[((x_max-i)*126)+((z-i)*21)+(y_max*3) + colorVal] and (x_max-i >= 0) and (z-i >= 0); i++){//loop through self color
			fittness += pow(2,3-i);
			numSelf+=1;
		}
		if(numSelf==3){
			fittness += 42;
			ANN->setFitness(ANN->getFitness() + fittness);
			return 14;//game won!	
		} 
	}else{
		for(; i<4 and inputs[((x_max-i)*126)+((z-i)*21)+(y_max*3)+ Invert(colorVal)] and (x_max-i >= 0) and (z-i >= 0); i++){//loop through other color
			fittness += pow(2,3-i);
		}
	}
	*/
	int vertical = -1;//(|)
	int horizontal = -1;//(-)
	int diagonal1 = -1;//(\)
	int diagonal2 = -1;//(/)
	int ic;//vertical
	int iic;//horizontal
	//check for vertical(|)
	for(ic = z;inputs[(x_max*126)+(ic*21)+(y_max*3)+ colorVal] == 1 && ic <= 5;ic++,vertical++);//Check down
	for(ic = z;inputs[(x_max*126)+(ic*21)+(y_max*3)+ colorVal] == 1 && ic >= 0;ic--,vertical++);//Check up
	if(vertical >= 4){
		//cout<<"Vertical: "<<x_max<<","<<z<<","<<y_max<<endl;
		return true;
	}
	//check for horizontal(-)
	for(iic = y_max;inputs[(x_max*126)+(z*21)+(iic*3)+ colorVal] == 1 && iic >= 0;iic--,horizontal++);//Check left
	for(iic = y_max;inputs[(x_max*126)+(z*21)+(iic*3)+ colorVal] == 1 && iic <= 6;iic++,horizontal++);//Check right
	if(horizontal >= 4){
		//cout<<"Horizontal_y: "<<x_max<<","<<z<<","<<y_max<<endl;
		return true;
	}
	//check for diagonal 1 (\)
	for(ic = z, iic= y_max;inputs[(x_max*126)+(ic*21)+(iic*3)+ colorVal] == 1 && ic>=0 && iic >=0; diagonal1 ++, ic --, iic --);//up and left
	for(ic = z, iic = y_max;inputs[(x_max*126)+(ic*21)+(iic*3)+ colorVal] == 1 && ic<=5 && iic <=6;diagonal1 ++, ic ++, iic ++);//down and right
	if(diagonal1 >= 4){
		//cout<<"Diagonal_y 1: "<<x_max<<","<<z<<","<<y_max<<endl;
		return true;
	}
	//check for diagonal 2(/)
	for(ic = z, iic= y_max;inputs[(x_max*126)+(ic*21)+(iic*3)+ colorVal] == 1 && ic>=0 && iic <= 6; diagonal2 ++, ic --, iic ++);//up and right
	for(ic = z, iic= y_max;inputs[(x_max*126)+(ic*21)+(iic*3)+ colorVal] && ic<=5 && iic >=0; diagonal2 ++, ic ++, iic --);//up and left
	if(diagonal2 >= 4){
		//cout<<"Diagonay 2: "<<x_max<<","<<z<<","<<y_max<<endl;
		return true;
	}
	vertical = -1;//(|)
	horizontal = -1;//(-)
	diagonal1 = -1;//(\)
	diagonal2 = -1;//(/)
	//iic;//horizontal
	//check for horiczontal(-)
	for(iic = x_max;inputs[(iic*126)+(z*21)+(y_max*3)+ colorVal] == 1 && iic >= 0;iic--,horizontal++);//Check left
	for(iic = x_max;inputs[(iic*126)+(z*21)+(y_max*3)+ colorVal] == 1 && iic <= 6;iic++,horizontal++);//Check ricght
	if(horizontal >= 4){
		//cout<<"Horizontal_x: "<<x_max<<","<<z<<","<<y_max<<endl;
		return true;
	}
	//check for diagonal 1 (\)
	for(ic = z, iic = x_max;inputs[(ic*126)+(ic*21)+(iic*3)+ colorVal] == 1 && ic>=0 && iic >=0; diagonal1 ++, ic --, iic --);//up and left
	for(ic = z, iic = x_max;inputs[(ic*126)+(ic*21)+(iic*3)+ colorVal] && ic<=5 && iic <=6;diagonal1 ++, ic ++, iic ++);//down and ricght
	if(diagonal1 >= 4){
		//cout<<"Diagonal_x 1: "<<x_max<<","<<z<<","<<y_max<<endl;
		return true;
	}
	//check for diagonal 2(/)
	for(ic = z, iic= x_max;inputs[(ic*126)+(ic*21)+(iic*3)+ colorVal] && ic>=0 && iic <= 6; diagonal2 ++, ic --, iic ++);//up and ricght
	for(ic = z, iic= x_max;inputs[(ic*126)+(ic*21)+(iic*3)+ colorVal] && ic<=5 && iic >=0; diagonal2 ++, ic ++, iic --);//up and left
	if(diagonal2 >= 4){
		//cout<<"Diagonal_x 2: "<<x_max<<","<<z<<","<<y_max<<endl;
		return true;
	}
	return false;
}

int Game::Invert(int i){
	if(i) return 0;
	else return 1;
}
#endif
