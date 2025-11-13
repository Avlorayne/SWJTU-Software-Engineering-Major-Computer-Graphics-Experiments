#ifndef INTERACTIVEGRAPHICS_H
#define INTERACTIVEGRAPHICS_H

#include <Windows.h>
#include "../models/GraphicsTypes.h"

// 全局变量声明
extern DrawMode g_currentMode;
extern std::vector<Point> g_currentPoints;
extern std::vector<GraphicShape> g_shapes;
extern bool g_isDrawing;

// 交互式图形函数
void InitializeInteractivePage(HWND hWnd);
void DrawInteractiveGraphics(HDC hdc);
LRESULT CALLBACK InteractivePageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ClearAllShapes();
void CompleteCurrentShape();  // 新增：完成当前图形

#endif // INTERACTIVEGRAPHICS_H