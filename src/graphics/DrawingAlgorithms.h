#ifndef DRAWINGALGORITHMS_H
#define DRAWINGALGORITHMS_H

#include <Windows.h>
#include <vector>
#include "../models/Point.h"

// 图形算法函数声明
void DrawLineMidpoint(HDC hdc, int x1, int y1, int x2, int y2);
void DrawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2);
void DrawCircleMidpoint(HDC hdc, int xc, int yc, int radius);
void DrawCircleBresenham(HDC hdc, int xc, int yc, int radius);
void DrawRectangle(HDC hdc, int x1, int y1, int x2, int y2);
void DrawPolyline(HDC hdc, const std::vector<Point>& points);
void DrawBSpline(HDC hdc, const std::vector<Point>& points);  // B样条曲线

#endif // DRAWINGALGORITHMS_H