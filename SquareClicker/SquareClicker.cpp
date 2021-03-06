// SquareClicker.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "SquareClicker.h"
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <windowsx.h>
#include <string.h>

#define MAX_LOADSTRING 100
const int  MAX_NUM_SQUARES =  5;
const int SQUARE_SIDE = 40;
const int SQUARE_SHOW_DELAY = 1200;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
//BOOL drawRect;
Square* squares;
int squaresCount;
int windowWidth;
int windowHeight;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	srand(time(NULL));
	squares = (Square*) malloc(sizeof(Square)* MAX_NUM_SQUARES);
	squaresCount = 0;
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SQUARECLICKER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	SetTimer(hWnd, 1, SQUARE_SHOW_DELAY, (TIMERPROC)NULL);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SQUARECLICKER));

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

	
    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SQUARECLICKER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SQUARECLICKER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   RECT rect;
   if (GetClientRect(hWnd, &rect))
   {
	   windowWidth = rect.right - rect.left;
	   windowHeight = rect.bottom - rect.top;
   }
   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int randomTop;
	int randomLeft;
	int clickedSquarePos;
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			for (int i = 1; i <= squaresCount; i++) {
				Square square = squares[i-1];
				HBRUSH hBrush; //создаём объект-кисть
				//RGB(255, 0, 67)
				hBrush = CreateSolidBrush(square.color); //задаём сплошную кисть, закрашенную цветом RGB
				SelectObject(hdc, hBrush); //делаем кисть активной
				Rectangle(hdc, square.left, square.top, square.right, square.bottom);
			}
			TCHAR text[14];
			swprintf(text,14, L"Lives left: %d/%s\n",  MAX_NUM_SQUARES - squaresCount, "\0");
			TextOut(hdc, 0, 0, text, 20);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_TIMER:
		if (squaresCount == MAX_NUM_SQUARES - 1) {
			KillTimer(hWnd, 1);
		}
	
		//TODO move this to a function
		
		Square newSquare = generateRandomSquare();

		while (squareIntersectsWithSomeOthers(newSquare)) {
			newSquare = generateRandomSquare();
		}

		squares[squaresCount] = newSquare;
		squaresCount++;
		RedrawWindow(hWnd,NULL,NULL,RDW_INVALIDATE);
		break;
	case  WM_LBUTTONUP:
		if (squaresCount < MAX_NUM_SQUARES) {
			clickedSquarePos = clickedSquareIndex(lParam);
			if (clickedSquarePos == squaresCount - 1) {
				squaresCount--;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
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

bool valueInRange(int value, int min, int max)
{
	return (value >= min) && (value <= max);
}

BOOL squaresOverlap(Square * first, Square * second) {
	bool xOverlap = valueInRange(first->left, second->left, second->left + SQUARE_SIDE) ||
		valueInRange(second->left, first->left,first->left + SQUARE_SIDE);

	bool yOverlap = valueInRange(first->top, second->top, second->top + SQUARE_SIDE) ||
		valueInRange(second->top, first->top, first->top + SQUARE_SIDE);

	return xOverlap && yOverlap;
}

Square generateRandomSquare() {
	Square newSquare;

	int randomX = ((windowWidth - SQUARE_SIDE) * rand()) / RAND_MAX;
	int randomY = ((windowHeight - SQUARE_SIDE) * rand()) / RAND_MAX;

	newSquare.top = randomY;
	newSquare.left = randomX;
	newSquare.bottom = newSquare.top + SQUARE_SIDE;
	newSquare.right = newSquare.left + SQUARE_SIDE;

	
	newSquare.color = RGB(
		(BYTE)(rand() % 255), 
		(BYTE)(rand() % 255), 
		(BYTE)(rand() % 255) 
	);
	return newSquare;
}

BOOL squareIntersectsWithSomeOthers(Square newSquare) {
	BOOL intersectsWithSomeOther = false;
	for (int i = 1; i <= squaresCount; i++) {
		intersectsWithSomeOther =
			squaresOverlap(&newSquare, &squares[i - 1]);
		if (intersectsWithSomeOther) {
			break;
		}
	}
	return intersectsWithSomeOther;
}

int clickedSquareIndex(LPARAM params) {
	int xPos = GET_X_LPARAM(params);
	int yPos = GET_Y_LPARAM(params);
	int index = -1;
	for (int i = 1; i <= squaresCount; i++) {
		if (squareContainsPoint(&squares[i - 1], xPos, yPos)) {
			index = i - 1;
		}
	}
	return index;
}

BOOL squareContainsPoint(Square * square, int x, int y) {
	return x > square->left && x < square->right &&
		y > square->top && y < square->bottom;
}