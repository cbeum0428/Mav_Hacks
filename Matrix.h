#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;
class Matrix {
	public:
		Matrix(int, int, bool);
		Matrix *transpose();
		void setValue(int, int, double);
		double getValue(int, int);
		void printToConsole();
		int getNumRows();
		int getNumCols();
	private:
		int rows, cols;
		vector< vector<double> > values;
};
Matrix::Matrix(int r, int c, bool b) {
	srand(time(NULL));
	rows = r;
	cols = c;
	for (r = 0;r < rows;r++) {
		vector<double> colValues;
		for (c = 0;c < cols;c++) {
			double r = 0.00;
			if (b) {
				r = (rand() % 10000) / 10000.00;
			}
			colValues.push_back(r);
		}
		values.push_back(colValues);
	}
}

Matrix *Matrix::transpose() {
	Matrix *m = new Matrix(cols, rows, false);
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			m->setValue(j, i, getValue(i, j));
		}
	}
	
	return m;
}

void Matrix::setValue(int r, int c, double v) {
	values[r][c] = v;
}

double Matrix::getValue(int r, int c) {
	return values.at(r).at(c);
}

void Matrix::printToConsole() {
	for (int r = 0;r < rows;r++) {
		for (int c = 0;c < cols;c++) {
			cout<<values.at(r).at(c)<<"\t\t";
		}
		cout<<endl;
	}
}

int Matrix::getNumRows() {
	return rows;
}

int Matrix::getNumCols() {
	return cols;
}
#endif
