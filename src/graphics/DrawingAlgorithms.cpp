#include "DrawingAlgorithms.h"
#include <cmath>

// 全局绘制颜色
COLORREF g_drawColor = RGB(0, 0, 0);

// 中点画线法
void DrawLineMidpoint(HDC hdc, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        SetPixel(hdc, x1, y1, g_drawColor);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Bresenham画线法
void DrawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        SetPixel(hdc, x1, y1, g_drawColor);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// 中点画圆法
void DrawCircleMidpoint(HDC hdc, int xc, int yc, int radius) {
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (x <= y) {
        SetPixel(hdc, xc + x, yc + y, g_drawColor);
        SetPixel(hdc, xc - x, yc + y, g_drawColor);
        SetPixel(hdc, xc + x, yc - y, g_drawColor);
        SetPixel(hdc, xc - x, yc - y, g_drawColor);
        SetPixel(hdc, xc + y, yc + x, g_drawColor);
        SetPixel(hdc, xc - y, yc + x, g_drawColor);
        SetPixel(hdc, xc + y, yc - x, g_drawColor);
        SetPixel(hdc, xc - y, yc - x, g_drawColor);

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

// Bresenham画圆法
void DrawCircleBresenham(HDC hdc, int xc, int yc, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y) {
        SetPixel(hdc, xc + x, yc + y, g_drawColor);
        SetPixel(hdc, xc - x, yc + y, g_drawColor);
        SetPixel(hdc, xc + x, yc - y, g_drawColor);
        SetPixel(hdc, xc - x, yc - y, g_drawColor);
        SetPixel(hdc, xc + y, yc + x, g_drawColor);
        SetPixel(hdc, xc - y, yc + x, g_drawColor);
        SetPixel(hdc, xc + y, yc - x, g_drawColor);
        SetPixel(hdc, xc - y, yc - x, g_drawColor);

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// 绘制矩形
void DrawRectangle(HDC hdc, int x1, int y1, int x2, int y2) {
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y1);
    LineTo(hdc, x2, y2);
    LineTo(hdc, x1, y2);
    LineTo(hdc, x1, y1);
}

// 绘制多段线
void DrawPolyline(HDC hdc, const std::vector<Point>& points) {
    if (points.size() < 2) return;

    MoveToEx(hdc, points[0].x, points[0].y, NULL);
    for (size_t i = 1; i < points.size(); i++) {
        LineTo(hdc, points[i].x, points[i].y);
    }
}

// 绘制B样条曲线
void DrawBSpline(HDC hdc, const std::vector<Point>& points) {
    // B样条曲线实现（简化版本）
    if (points.size() < 4) return;

    // 这里实现一个简单的B样条曲线算法
    for (size_t i = 0; i < points.size() - 3; i++) {
        const Point& p0 = points[i];
        const Point& p1 = points[i + 1];
        const Point& p2 = points[i + 2];
        const Point& p3 = points[i + 3];

        // 简化的B样条绘制
        MoveToEx(hdc, (p0.x + 4 * p1.x + p2.x) / 6, (p0.y + 4 * p1.y + p2.y) / 6, NULL);
        for (double t = 0; t <= 1; t += 0.05) {
            double t2 = t * t;
            double t3 = t2 * t;
            
            double b0 = (-t3 + 3 * t2 - 3 * t + 1) / 6.0;
            double b1 = (3 * t3 - 6 * t2 + 4) / 6.0;
            double b2 = (-3 * t3 + 3 * t2 + 3 * t + 1) / 6.0;
            double b3 = t3 / 6.0;
            
            int x = static_cast<int>(b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x);
            int y = static_cast<int>(b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y);
            
            LineTo(hdc, x, y);
        }
    }
}