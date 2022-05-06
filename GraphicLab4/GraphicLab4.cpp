// GraphicLab4.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "GraphicLab4.h"
#include "MyObject.h"
#include <string>


#define MAX_LOADSTRING 100


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

MyObject pyramid(5, "pyramid.txt");
unsigned int axis = 0, projection = 0;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GRAPHICLAB4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICLAB4));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICLAB4));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GRAPHICLAB4);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 900, 600, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN:
	{
		int key = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (key)
		{
			//перемещение 
		case 0x57://w
			if (axis == 1)
			{
				pyramid.move(0, 0, 30);
			}
			else
			{
				pyramid.move(0, -30, 0);
			}
			break;
		case 0x53://s
			if (axis == 1)
			{
				pyramid.move(0, 0, -30);
			}
			else
			{
				pyramid.move(0, 30, 0);
			}
			break;
		case 0x41://a
			if (axis == 0)
			{
				pyramid.move(0, 0, -30);
			}
			else
			{
				pyramid.move(-30, 0, 0);
			}
			break;
		case 0x44://d
			if (axis == 0)
			{
				pyramid.move(0, 0, 30);
			}
			else
			{
				pyramid.move(30, 0, 0);
			}
			break;
			// поворот 
		case 0x45://e
			pyramid.rotate(gradToRad(15), axis);
			break;
		case 0x51://q
			pyramid.rotate(gradToRad(-15), axis);
			break;
		case 0x52://r
			pyramid.scale(1.2, 1.2, 1.2);// увеличение размеров
			break;
		case 0x46://f
			pyramid.scale(0.8, 0.8, 0.8);// умегьшение размеров
			break;
		case 0x43://c
			axis = (axis + 1) % 3;//0-X,1-Y,2-Z
			break;
		case 0x4b://k
			projection = (projection + 1) % 2;
			break;
		case 0x1B://ESC
			PostQuitMessage(0);
			break;
		}
	}
	InvalidateRect(hWnd, NULL, true);
	UpdateWindow(hWnd);
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);// получение десриптора графического устройства

		// создание совместимого битмапа для создания буфера(совпадает с клиенсткой областью), 
		//в котором будет предварительная отрисовка окна для избежания мерцаний из-за постепеной отрисовки окна
		HDC bufferDC = CreateCompatibleDC(hdc);
		HBITMAP bufferBM = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);

		SelectObject(bufferDC, bufferBM);

		//отрисовка:
		// 
		//Заполнение буфера белым цветом
		FillRect(bufferDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		switch (axis)
		{
		case 0:
			TextOut(bufferDC, 10, 10, L"Axis:X", 7);
			break;
		case 1:
			TextOut(bufferDC, 10, 10, L"Axis:Y", 7);
			break;
		case 2:
			TextOut(bufferDC, 10, 10, L"Axis:Z", 7);
			break;
		}
		std::string X = std::to_string(pyramid.getMiddleX());
		std::string Y = std::to_string(pyramid.getMiddleY());
		std::string Z = std::to_string(pyramid.getMiddleZ());
		TextOutA(bufferDC, 10, 25, X.c_str(), 7);
		TextOutA(bufferDC, 10, 40, Y.c_str(), 7);
		TextOutA(bufferDC, 10, 55, Z.c_str(), 7);
		Matrix cameraMatrix(1,1);
		if (projection)
		{
			double angle = gradToRad(30);

			cameraMatrix = {
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
			cameraMatrix = cameraMatrix * rotM;
		}
		else
		{
			cameraMatrix = {
				 {1,0,0,0},
				 {0,1,0,0},
				 {0,0,0,0},
				 {0,0,0,1},
			};
		}

		pyramid.draw(bufferDC, RGB(0, 0, 255), cameraMatrix);// отрисовка объекта в bufferDC и заполнение синим цветом

		//Загрузка буфера в битмап графического устройства и освобождение памяти выделенной на буфер
		BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, bufferDC, 0, 0, SRCCOPY);
		DeleteDC(bufferDC);
		DeleteObject(bufferBM);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
