#include "Matrix.h"
Matrix::~Matrix()
{
	for (size_t i = 0; i < rowsCnt; i++)
	{
		delete[] data[i];
	}
	delete[] data;
}

Matrix::Matrix(unsigned int rows, unsigned int cols) :rowsCnt(rows), colsCnt(cols)
{
	data = new double* [rowsCnt];
	for (size_t i = 0; i < rowsCnt; i++)
	{
		data[i] = new double[colsCnt];
	}
}
Matrix::Matrix(const std::initializer_list<std::initializer_list<double>>& MatrixInit)
{
	int i = 0;
	rowsCnt=MatrixInit.size();
	data = new double* [rowsCnt];
	for (auto& row : MatrixInit)
	{
		colsCnt=row.size();
		data[i] = new double[colsCnt];
		int j = 0;
		for (auto& element:row)
		{
			data[i][j]=element;
			++j;
		}
		++i;
	}
}

Matrix::Matrix(const Matrix& orig) :colsCnt(orig.colsCnt), rowsCnt(orig.rowsCnt)
{
	data = new double* [rowsCnt];
	for (size_t i = 0; i < rowsCnt; i++)
	{
		data[i] = new double[colsCnt];
		for (size_t j = 0; j < colsCnt; j++)
		{
			data[i][j] = orig.data[i][j];
		}
	}
}

void Matrix::fill(double value)
{
	for (size_t i = 0; i < rowsCnt; i++)
	{
		for (size_t j = 0; j < colsCnt; j++)
		{
			data[i][j] = value;
		}
	}
}

MatrixRow Matrix::operator[](unsigned int index)
{
	MatrixRow result(data[index],colsCnt);
	return result;
}

Matrix Matrix::operator*(Matrix& rightValue)
{
	Matrix result(rowsCnt, rightValue.colsCnt);
	result.fill(0);
	for (size_t i = 0; i < rowsCnt; i++)// умножение матрицы с координатами на матрицу преобразования
	{
		for (size_t j = 0; j < rightValue.colsCnt; j++)
		{
			for (size_t k = 0; k < colsCnt; k++)
			{
				result[i][j] += data[i][k] * rightValue[k][j];
			}

		}
	}
	return result;
}

Matrix& Matrix::operator=(const Matrix& rightValue)
{
	if (this == &rightValue)
		return *this;

	if (colsCnt == rightValue.colsCnt && rowsCnt == rightValue.rowsCnt)
	{
		for (size_t i = 0; i < rowsCnt; i++)
		{
			for (size_t j = 0; j < colsCnt; j++)
			{
				data[i][j] = rightValue.data[i][j];
			}
		}
		return *this;
	}

	for (size_t i = 0; i < rowsCnt; i++)
	{
		delete[] data[i];
	}
	delete[] data;
	rowsCnt = rightValue.rowsCnt;
	colsCnt = rightValue.colsCnt;
	data = new double* [rowsCnt];
	for (size_t i = 0; i < rowsCnt; i++)
	{
		data[i] = new double[colsCnt];
		for (size_t j = 0; j < colsCnt; j++)
		{
			data[i][j] = rightValue.data[i][j];
		}
	}
	return *this;
}
Matrix& Matrix::operator=(const std::initializer_list<std::initializer_list<double>>& MatrixInit)
{
	for (size_t i = 0; i < rowsCnt; i++)
	{
		delete[] data[i];
	}
	delete[] data;
	int i = 0;
	rowsCnt = MatrixInit.size();
	data = new double* [rowsCnt];
	for (auto& row : MatrixInit)
	{
		colsCnt = row.size();
		data[i] = new double[colsCnt];
		int j = 0;
		for (auto& element : row)
		{
			data[i][j] = element;
			++j;
		}
		++i;
	}
	return *this;
}

void MatrixRow::operator=(std::initializer_list<double>& newRow)
{
	int count = 0;
	for (auto& element : newRow)
	{
		row[count] = element;
		++count;
	}
}

