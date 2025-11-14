#ifndef GRAPHICSTYPES_H
#define GRAPHICSTYPES_H

#include "Point.h"
#include <vector>

// 图形绘制模式枚举
enum DrawMode {
    MODE_NONE,
    MODE_LINE_MIDPOINT,
    MODE_LINE_BRESENHAM,
    MODE_CIRCLE_MIDPOINT,
    MODE_CIRCLE_BRESENHAM,
    MODE_RECTANGLE,
    MODE_POLYLINE,
    MODE_BSPLINE,
    MODE_FILL_SCANLINE,    // 扫描线填充
    MODE_FILL_FENCE        // 栅栏填充
};

// 图形结构体
struct GraphicShape {
    DrawMode mode;
    std::vector<Point> points;
    COLORREF color;

    GraphicShape(DrawMode m, const std::vector<Point>& p, COLORREF c = RGB(255, 0, 0))
        : mode(m), points(p), color(c) {}
};

#endif // GRAPHICSTYPES_H