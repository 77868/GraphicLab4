#pragma once
#include "Matrix.h"
#include <Windows.h>

#define PI 3.14159265358979323846

// макрос преобразования X градусов в радианы 
#define gradToRad(x) x * PI/180 



class MyObject
{
private:
	Matrix cords;
	unsigned int** polygons;
	unsigned int polygCount,pointCount;
	
public:
	MyObject(unsigned int pointCnt,const char* fileName);
	void rotate(double angle,unsigned int axis);
	void scale(double scaleX,double scaleY,double scaleZ);
	void move(double moveX, double moveY, double moveZ);
	double getMiddleX();
	double getMiddleY();
	double getMiddleZ();
	void draw(HDC hdc, COLORREF color, Matrix& transformM);

};

