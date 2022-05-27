#include "MyObject.h"
#include <Windows.h>
#include <cmath>
#include <fstream>

bool MyObject::isVisible(int polygIndex, Matrix& renderCords)
{
	double V1X = cords[polygons[polygIndex][0]][0] - cords[polygons[polygIndex][1]][0];//координаты вектора 1 преднадлежещего исследуемой плоскости
	double V1Y = cords[polygons[polygIndex][0]][1] - cords[polygons[polygIndex][1]][1];
	double V1Z = cords[polygons[polygIndex][0]][2] - cords[polygons[polygIndex][1]][2];

	double V2X = cords[polygons[polygIndex][1]][0] - cords[polygons[polygIndex][2]][0];//координаты вектора 2 преднадлежещего исследуемой плоскости
	double V2Y = cords[polygons[polygIndex][1]][1] - cords[polygons[polygIndex][2]][1];
	double V2Z = cords[polygons[polygIndex][1]][2] - cords[polygons[polygIndex][2]][2];

	double A = V1Y * V2Z - V2Y * V1Z;//нахождение коофицентов уравнения плоскости
	double B = V1Z * V2X - V2Z * V1X;
	double C = V1X * V2Y - V2X * V1Y;
	double D = -(A* cords[polygons[polygIndex][0]][0] +B* cords[polygons[polygIndex][0]][1] +C* cords[polygons[polygIndex][0]][2]);

	double sign = ((A*getMiddleX() + B*getMiddleY() + C*getMiddleZ()+D)<0) ? 1 : -1;//нахождение направления нормали относительно цетра фигуры

	A *= sign;
	B *= sign;
	C *= sign;
	D *= sign;

	if ( (A*0 + B*0 + C*300 + D) < 0)
		return 0;
	return 1;
}

void MyObject::fill(int polygIndex,Matrix& renderCords,HDC hdc)
{
	if (renderCords[polygons[polygIndex][0]][1] > renderCords[polygons[polygIndex][1]][1]) std::swap(polygons[polygIndex][0], polygons[polygIndex][1]);
	if (renderCords[polygons[polygIndex][1]][1] > renderCords[polygons[polygIndex][2]][1]) std::swap(polygons[polygIndex][1], polygons[polygIndex][2]);
	if (renderCords[polygons[polygIndex][0]][1] > renderCords[polygons[polygIndex][1]][1]) std::swap(polygons[polygIndex][0], polygons[polygIndex][1]);

	HPEN newPen = CreatePen(PS_SOLID, NULL, RGB(0, 255, 0));// создание и выбор кисти необходимого цвета
	HGDIOBJ prevPen = SelectObject(hdc, newPen);

	double x1, x2;
	for (int y = renderCords[polygons[polygIndex][0]][1]; y < renderCords[polygons[polygIndex][1]][1]; y++) {
		if ((renderCords[polygons[polygIndex][1]][1] - renderCords[polygons[polygIndex][0]][1]) < 1 || (renderCords[polygons[polygIndex][2]][1] - renderCords[polygons[polygIndex][0]][1]) < 1) {
			continue;
		}
		x1 = (((y - renderCords[polygons[polygIndex][0]][1]) * (renderCords[polygons[polygIndex][1]][0] - renderCords[polygons[polygIndex][0]][0])) / (renderCords[polygons[polygIndex][1]][1] - renderCords[polygons[polygIndex][0]][1])) + renderCords[polygons[polygIndex][0]][0];
		x2 = (((y - renderCords[polygons[polygIndex][0]][1]) * (renderCords[polygons[polygIndex][2]][0] - renderCords[polygons[polygIndex][0]][0])) / (renderCords[polygons[polygIndex][2]][1] - renderCords[polygons[polygIndex][0]][1])) + renderCords[polygons[polygIndex][0]][0];
		MoveToEx(hdc, static_cast<int>(round(x1)), y, NULL);
		LineTo(hdc, static_cast<int>(round(x2)), y);
	}
	for (int y = renderCords[polygons[polygIndex][1]][1]; y < renderCords[polygons[polygIndex][2]][1]; y++) {
		if ((renderCords[polygons[polygIndex][2]][1] - renderCords[polygons[polygIndex][1]][1]) == 0 || (renderCords[polygons[polygIndex][2]][1] - renderCords[polygons[polygIndex][0]][1]) == 0) {
			continue;
		}
		x1 = (((y - renderCords[polygons[polygIndex][1]][1]) * (renderCords[polygons[polygIndex][2]][0] - renderCords[polygons[polygIndex][1]][0])) / (renderCords[polygons[polygIndex][2]][1] - renderCords[polygons[polygIndex][1]][1])) + renderCords[polygons[polygIndex][1]][0];
		x2 = (((y - renderCords[polygons[polygIndex][0]][1]) * (renderCords[polygons[polygIndex][2]][0] - renderCords[polygons[polygIndex][0]][0])) / (renderCords[polygons[polygIndex][2]][1] - renderCords[polygons[polygIndex][0]][1])) + renderCords[polygons[polygIndex][0]][0];
		MoveToEx(hdc, static_cast<int>(round(x1)), y, NULL);
		LineTo(hdc, static_cast<int>(round(x2)), y);
	}

	SelectObject(hdc, prevPen);// возращение старой кисти и освобождение новой
	DeleteObject(newPen);

	newPen = CreatePen(PS_SOLID, NULL, RGB(0, 0, 0));// создание и выбор кисти необходимого цвета
	prevPen = SelectObject(hdc, newPen);
	// отрисовка рёбер треугольника
	MoveToEx(hdc, static_cast<int>(renderCords[polygons[polygIndex][0]][0]), static_cast<int>(renderCords[polygons[polygIndex][0]][1]), nullptr);
	LineTo(hdc, static_cast<int>(renderCords[polygons[polygIndex][1]][0]), static_cast<int>(renderCords[polygons[polygIndex][1]][1]));

	MoveToEx(hdc, static_cast<int>(renderCords[polygons[polygIndex][1]][0]), static_cast<int>(renderCords[polygons[polygIndex][1]][1]), nullptr);
	LineTo(hdc, static_cast<int>(renderCords[polygons[polygIndex][2]][0]), static_cast<int>(renderCords[polygons[polygIndex][2]][1]));

	MoveToEx(hdc, static_cast<int>(renderCords[polygons[polygIndex][0]][0]), static_cast<int>(renderCords[polygons[polygIndex][0]][1]), nullptr);
	LineTo(hdc, static_cast<int>(renderCords[polygons[polygIndex][2]][0]), static_cast<int>(renderCords[polygons[polygIndex][2]][1]));
	SelectObject(hdc, prevPen);// возращение старой кисти и освобождение новой
	DeleteObject(newPen);
}

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

void MyObject::draw(HDC hdc)
{
	Matrix renderCords(cords);


	for (size_t i = 0; i < pointCount; i++)
	{
		renderCords[i][0] = 300 * renderCords[i][0] / renderCords[i][2] +450;
		renderCords[i][1] = 300 * renderCords[i][1] / renderCords[i][2] +300;
	}
	
	for (size_t i = 0; i < polygCount; i++)
	{
		if (isVisible(i, renderCords))
		{
			fill(i,renderCords,hdc);
		}
	}
}



