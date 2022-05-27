#pragma once
#include "Matrix.h"
#include <Windows.h>

#define PI 3.14159265358979323846

// ������ �������������� X �������� � ������� 
#define gradToRad(x) x * PI/180 



class MyObject
{
private:
	Matrix cords;
	unsigned int** polygons;
	unsigned int polygCount,pointCount;
	bool isVisible(int polygIndex, Matrix& renderCords);
	void fill(int polygIndex, Matrix& renderCords, HDC hdc);
	
public:
	MyObject(unsigned int pointCnt,const char* fileName);
	void rotate(double angle,unsigned int axis);
	void scale(double scaleX,double scaleY,double scaleZ);
	void move(double moveX, double moveY, double moveZ);
	double getMiddleX();
	double getMiddleY();
	double getMiddleZ();
	void draw(HDC hdc);

};

