#ifndef UICOMPONENTS_H
#define UICOMPONENTS_H

#include <Windows.h>
#include "../models/GraphicsTypes.h"

// UI组件函数
void AddToolButton(HWND hWnd, int x, int y, int width, int height, const wchar_t* text, int id);
void AddModeButton(HWND hWnd, int x, int y, int width, int height, const wchar_t* text, DrawMode mode);
void DrawControlPoints(HDC hdc, const std::vector<Point>& points, COLORREF color);
void DrawBackground(HDC hdc, HWND hWnd, COLORREF color = RGB(255, 255, 255));

#endif // UICOMPONENTS_H