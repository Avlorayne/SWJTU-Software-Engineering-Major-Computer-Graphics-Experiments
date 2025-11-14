#include "DrawingAlgorithms.h"
#include <cmath>
#include <algorithm>

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

// 计算B样条基函数
double BSplineBasis(int i, int k, double t, const std::vector<double>& knots) {
    if (k == 0) {
        return (t >= knots[i] && t < knots[i + 1]) ? 1.0 : 0.0;
    }

    double denom1 = knots[i + k] - knots[i];
    double denom2 = knots[i + k + 1] - knots[i + 1];

    double term1 = 0.0;
    double term2 = 0.0;

    if (denom1 != 0.0) {
        term1 = ((t - knots[i]) / denom1) * BSplineBasis(i, k - 1, t, knots);
    }

    if (denom2 != 0.0) {
        term2 = ((knots[i + k + 1] - t) / denom2) * BSplineBasis(i + 1, k - 1, t, knots);
    }

    return term1 + term2;
}

// 生成均匀节点向量
std::vector<double> GenerateUniformKnots(int n, int k) {
    std::vector<double> knots(n + k + 1);
    for (int i = 0; i <= n + k; i++) {
        knots[i] = static_cast<double>(i);
    }
    return knots;
}

// 绘制B样条曲线
void DrawBSpline(HDC hdc, const std::vector<Point>& points) {
    if (points.size() < 4) {
        // 如果控制点太少，直接绘制折线
        DrawPolyline(hdc, points);
        return;
    }

    int n = points.size() - 1;  // 控制点索引从0到n
    int k = 3;                  // 3次B样条（最常用）

    // 生成均匀节点向量
    std::vector<double> knots = GenerateUniformKnots(n, k);

    // 设置曲线精度
    const int segments = 100;
    bool firstPoint = true;
    int lastX = 0, lastY = 0;

    // 计算参数t的范围
    double t_min = knots[k];
    double t_max = knots[n + 1];
    double t_step = (t_max - t_min) / segments;

    for (int s = 0; s <= segments; s++) {
        double t = t_min + s * t_step;

        double x = 0.0, y = 0.0;

        // 计算曲线上的点
        for (int i = 0; i <= n; i++) {
            double basis = BSplineBasis(i, k, t, knots);
            x += points[i].x * basis;
            y += points[i].y * basis;
        }

        int currentX = static_cast<int>(x + 0.5);
        int currentY = static_cast<int>(y + 0.5);

        if (!firstPoint) {
            // 使用Bresenham算法绘制线段，使曲线更平滑
            DrawLineBresenham(hdc, lastX, lastY, currentX, currentY);
        } else {
            firstPoint = false;
        }

        lastX = currentX;
        lastY = currentY;
    }
}