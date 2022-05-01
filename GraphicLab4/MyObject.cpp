#include "MyObject.h"
#include <Windows.h>
#include <cmath>
#include <fstream>

MyObject::MyObject(unsigned int pointCnt, const char* fileName) : cords(pointCnt, 4)
{
	std::ifstream in(fileName);
	if (!in.is_open() || in.eof())
		return;
	in >> pointCount;
	for (size_t i = 0; i < pointCount; i++)
	{
		in >> cords[i][0] >> cords[i][1] >> cords[i][2];
		cords[i][3] = 1.0f;
	}
	in >> polygCount;
	polygons = new unsigned int* [polygCount];
	for (size_t i = 0; i < polygCount; i++)
	{
		polygons[i] = new unsigned int[3];
		in >> polygons[i][0] >> polygons[i][1] >> polygons[i][2];
	}
}



double MyObject::getMiddleX()
{
	double result=0;
	for (size_t i = 0; i < pointCount; i++)
	{
		result += cords[i][0];
	}
	return result / pointCount;
}
double MyObject::getMiddleY()
{
	double result = 0;
	for (size_t i = 0; i < pointCount; i++)
	{
		result += cords[i][1];
	}
	return result / pointCount;
}
double MyObject::getMiddleZ()
{
	double result = 0;
	for (size_t i = 0; i < pointCount; i++)
	{
		result += cords[i][2];
	}
	return result / pointCount;
}

void MyObject::move(double moveX,double moveY,double moveZ)
{
	Matrix moveM{
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{moveX,moveY,moveZ,1}
	};
	cords = cords * moveM; 
}
void MyObject::scale(double scaleX, double scaleY, double scaleZ)
{
	Matrix transformMatrix(4, 4);
	Matrix moveM{
		{1,0,0,0},
		{0,1,0,0,0},
		{0,0,1,0},
		{-getMiddleX(),-getMiddleY(),-getMiddleZ(),1},
	};
	Matrix scaleM{
		{scaleX,0,0,0},
		{0,scaleY,0,0},
		{0,0,scaleZ,0},
		{0,0,0,1}
	};
	transformMatrix = moveM * scaleM;
	moveM[3][0] *= -1;
	moveM[3][1] *= -1;
	moveM[3][2] *= -1;
	transformMatrix = transformMatrix * moveM;
	cords = cords * transformMatrix;
}
void MyObject::rotate(double angle, unsigned int axis)
{
	Matrix transformM(4, 4);
	Matrix moveM{
		{1,0,0,0},
		{0,1,0,0,0},
		{0,0,1,0},
		{-getMiddleX(),-getMiddleY(),-getMiddleZ(),1},
	};

	switch (axis)
	{
	case 0:
	{
		Matrix rotM{
			{1,0,0,0},
			{0,cos(angle),sin(angle),0},
			{0,-sin(angle),cos(angle),0},
			{0,0,0,1}
		};
		transformM = moveM*rotM;
	}
		break;
	case 1:
	{
		Matrix rotM
		{
			{cos(angle),0,sin(angle),0},
			{0,1,0,0},
			{-sin(angle),0,cos(angle),0},
			{0,0,0,1}
		};
		transformM = moveM * rotM;
	}
		break;
	case 2:
	{
		Matrix rotM
		{
			{cos(angle),sin(angle),0,0},
			{-sin(angle),cos(angle),0,0},
			{0,0,1,0},
			{0,0,0,1}
		};
		transformM = moveM * rotM;
	}
		break;
	default:
		return;
	}
	
	moveM[3][0] *= -1;
	moveM[3][1] *= -1;
	moveM[3][2] *= -1;
	transformM = transformM*moveM;
	cords = cords * transformM;
}

void MyObject::draw(HDC hdc, COLORREF color, Matrix& transformM)
{
	HPEN newPen = CreatePen(PS_SOLID, NULL, color);// создание и выбор кисти необходимого цвета
	HGDIOBJ prevPen = SelectObject(hdc, newPen);

	Matrix renderCords(cords);
	/*double angle = gradToRad(45);
	
	Matrix transformM{
			{cos(angle),0,sin(angle),0},
			{0,1,0,0},
			{-sin(angle),0,cos(angle),0},
			{0,0,0,1}
		
	};
	angle = asin(tan(gradToRad(30)));
	Matrix rotM{
			{1,0,0,0},
			{0,cos(angle),sin(angle),0},
			{0,-sin(angle),cos(angle),0},
			{0,0,0,1}
	};
	transformM = transformM * rotM;*/


	/*Matrix transformM{
	{1,0,0,0},
	{0,1,0,0},
	{0,0,0,0},
	{0,0,0,1},
	};*/
	renderCords = renderCords * transformM;
	
	for (size_t i = 0; i < polygCount; i++)
	{
		// отрисовка рёбер треугольника
		MoveToEx(hdc, static_cast<int>(renderCords[polygons[i][0]][0]), static_cast<int>(renderCords[polygons[i][0]][1]), nullptr);
		LineTo(hdc, static_cast<int>(renderCords[polygons[i][1]][0]), static_cast<int>(renderCords[polygons[i][1]][1]));

		MoveToEx(hdc, static_cast<int>(renderCords[polygons[i][1]][0]), static_cast<int>(renderCords[polygons[i][1]][1]), nullptr);
		LineTo(hdc, static_cast<int>(renderCords[polygons[i][2]][0]), static_cast<int>(renderCords[polygons[i][2]][1]));

		MoveToEx(hdc, static_cast<int>(renderCords[polygons[i][0]][0]), static_cast<int>(renderCords[polygons[i][0]][1]), nullptr);
		LineTo(hdc, static_cast<int>(renderCords[polygons[i][2]][0]), static_cast<int>(renderCords[polygons[i][2]][1]));

	}

	SelectObject(hdc, prevPen);// возращение старой кисти и освобождение новой
	DeleteObject(newPen);
}



