#ifndef FILLALGORITHMS_H
#define FILLALGORITHMS_H

#include <Windows.h>
#include <vector>
#include "../models/Point.h"
#include "../models/GraphicsTypes.h"

// 填充算法函数声明
void ScanLineFill(HDC hdc, const std::vector<Point>& points, COLORREF fillColor);
void FenceFill(HDC hdc, const std::vector<Point>& points, COLORREF fillColor);
void FloodFill4(HDC hdc, int x, int y, COLORREF oldColor, COLORREF newColor);
void FloodFill8(HDC hdc, int x, int y, COLORREF oldColor, COLORREF newColor);

// 区域填充函数
bool FillAreaAtPoint(HDC hdc, int x, int y, DrawMode fillMode, COLORREF fillColor);

// 辅助函数
bool IsPointInPolygon(int x, int y, const std::vector<Point>& points);
std::vector<Point> FindClosedShapeAtPoint(const std::vector<GraphicShape>& shapes, int x, int y);

#endif // FILLALGORITHMS_H