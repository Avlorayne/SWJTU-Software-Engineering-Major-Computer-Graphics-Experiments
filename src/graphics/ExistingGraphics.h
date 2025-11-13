#ifndef EXISTINGGRAPHICS_H
#define EXISTINGGRAPHICS_H

#include <Windows.h>

// 工程图绘制函数
void DrawExistingGraphics(HDC hdc);
void DrawDimensionLine(HDC hdc, int x1, int y1, int x2, int y2, LPCWSTR text);

#endif // EXISTINGGRAPHICS_H