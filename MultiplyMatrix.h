#ifndef MULTIPLYMATRIX_H
#define MULTIPLYMATRIX_H
#include <iostream>
#include "Matrix.h"
using namespace std;
class MultiplyMatrix {
	public:
		MultiplyMatrix(Matrix *, Matrix *);
		
		Matrix *execute();
	private:
	Matrix *a;
	Matrix *b;
	Matrix *c;
};

MultiplyMatrix::MultiplyMatrix(Matrix *am, Matrix *bm) {
	a = am;
	b = bm;
	
	if (a->getNumCols() != b->getNumRows()) {
		cout<<"Cant multiply these together..."<<endl;
		exit(0);
	}
	c = new Matrix(a->getNumRows(), b->getNumCols(), false);
}

Matrix *MultiplyMatrix::execute() {
	for (int i = 0;i < a->getNumRows();i++) {
		for (int j = 0;j < b->getNumCols(); j++) {
			for (int k = 0;k < b->getNumRows();k++) {
				double p = a->getValue(i, k) * b->getValue(k, j);
				double newValue = c->getValue(i, j) + p;
				c->setValue(i, j, newValue);
			}
		}
	}
	return c;
}
#endif
