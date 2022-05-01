#pragma once
#include <iostream>
class MatrixRow
{
private:
	double* row;
	unsigned int colsCnt;
public:
	MatrixRow(double* row,unsigned int colsCnt) :row(row),colsCnt(colsCnt) {};
	void operator=(std::initializer_list<double>& newRow);
	double& operator[](unsigned int index)
	{
		return row[index];
	}
};


class Matrix
{
private:
	double** data;	
	unsigned int rowsCnt, colsCnt;
public:
	~Matrix();
	Matrix(unsigned int rows, unsigned int cols);
	Matrix(const Matrix& orig);
	Matrix(const std::initializer_list<std::initializer_list<double>>& MatrixInit);
	void fill(double value);
	Matrix operator*(Matrix& rightValue);
	Matrix& operator=(const Matrix& rightValue);
	Matrix& operator=(const std::initializer_list<std::initializer_list<double>>& MatrixInit);
	MatrixRow operator[](unsigned int index);
};
