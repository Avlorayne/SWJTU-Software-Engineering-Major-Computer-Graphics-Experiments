#include "FillAlgorithms.h"
#include <algorithm>
#include <vector>
#include <stack>
#include <cmath>
#include <queue>

using namespace std;

// 边结构体
struct Edge {
    int y_max;
    int y_min;
    double x;
    double slope;

    Edge(int ymax, int ymin, double x_val, double s)
        : y_max(ymax), y_min(ymin), x(x_val), slope(s) {}

    bool operator<(const Edge& other) const {
        if (y_min != other.y_min) return y_min < other.y_min;
        return x < other.x;
    }
};

// 扫描线填充算法
void ScanLineFill(HDC hdc, const std::vector<Point>& points, COLORREF fillColor) {
    if (points.size() < 3) return;

    vector<Edge> edgeTable;

    // 建立边表
    for (size_t i = 0; i < points.size(); i++) {
        size_t next_i = (i + 1) % points.size();
        const Point& p1 = points[i];
        const Point& p2 = points[next_i];

        if (p1.y == p2.y) continue;

        int y_max = max(p1.y, p2.y);
        int y_min = min(p1.y, p2.y);
        double x_val = (p1.y < p2.y) ? p1.x : p2.x;
        double slope = (p2.x - p1.x) / (double)(p2.y - p1.y);

        edgeTable.push_back(Edge(y_max, y_min, x_val, slope));
    }

    // 对边表排序
    sort(edgeTable.begin(), edgeTable.end());

    vector<Edge> activeEdgeTable;

    // 计算扫描线范围
    int y_min = INT_MAX, y_max = INT_MIN;
    for (const auto& point : points) {
        if (point.y < y_min) y_min = point.y;
        if (point.y > y_max) y_max = point.y;
    }

    // 扫描线算法
    for (int y = y_min; y <= y_max; y++) {
        // 添加新边
        for (auto it = edgeTable.begin(); it != edgeTable.end();) {
            if (it->y_min == y) {
                activeEdgeTable.push_back(*it);
                it = edgeTable.erase(it);
            } else {
                ++it;
            }
        }

        // 删除过期边
        for (auto it = activeEdgeTable.begin(); it != activeEdgeTable.end();) {
            if (it->y_max == y) {
                it = activeEdgeTable.erase(it);
            } else {
                ++it;
            }
        }

        // 排序活性边表
        sort(activeEdgeTable.begin(), activeEdgeTable.end(),
             [](const Edge& a, const Edge& b) { return a.x < b.x; });

        // 填充扫描线
        for (size_t i = 0; i < activeEdgeTable.size(); i += 2) {
            if (i + 1 < activeEdgeTable.size()) {
                int x_start = static_cast<int>(activeEdgeTable[i].x + 0.5);
                int x_end = static_cast<int>(activeEdgeTable[i + 1].x + 0.5);

                for (int x = x_start; x <= x_end; x++) {
                    SetPixel(hdc, x, y, fillColor);
                }
            }
        }

        // 更新x坐标
        for (auto& edge : activeEdgeTable) {
            edge.x += edge.slope;
        }
    }
}

// 栅栏填充算法
void FenceFill(HDC hdc, const std::vector<Point>& points, COLORREF fillColor) {
    if (points.size() < 3) return;

    // 计算边界框
    int min_x = INT_MAX, max_x = INT_MIN;
    int min_y = INT_MAX, max_y = INT_MIN;

    for (const auto& point : points) {
        if (point.x < min_x) min_x = point.x;
        if (point.x > max_x) max_x = point.x;
        if (point.y < min_y) min_y = point.y;
        if (point.y > max_y) max_y = point.y;
    }

    // 选择栅栏线
    int fence_x = (min_x + max_x) / 2;

    // 绘制栅栏线（可视化）
    HPEN hFencePen = CreatePen(PS_DOT, 1, RGB(128, 128, 128));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hFencePen);
    MoveToEx(hdc, fence_x, min_y, NULL);
    LineTo(hdc, fence_x, max_y);
    SelectObject(hdc, hOldPen);
    DeleteObject(hFencePen);

    // 对每条扫描线进行填充
    for (int y = min_y; y <= max_y; y++) {
        vector<int> intersections;

        // 计算交点
        for (size_t i = 0; i < points.size(); i++) {
            size_t next_i = (i + 1) % points.size();
            const Point& p1 = points[i];
            const Point& p2 = points[next_i];

            if ((p1.y <= y && p2.y > y) || (p2.y <= y && p1.y > y)) {
                double t = (y - p1.y) / (double)(p2.y - p1.y);
                int x_intersect = static_cast<int>(p1.x + t * (p2.x - p1.x) + 0.5);
                intersections.push_back(x_intersect);
            }
        }

        // 排序并填充
        sort(intersections.begin(), intersections.end());

        for (size_t i = 0; i < intersections.size(); i += 2) {
            if (i + 1 < intersections.size()) {
                int x_start = intersections[i];
                int x_end = intersections[i + 1];

                // 根据栅栏位置填充
                if (x_end < fence_x) {
                    // 左侧填充
                    for (int x = x_start; x <= x_end; x++) {
                        SetPixel(hdc, x, y, fillColor);
                    }
                } else if (x_start > fence_x) {
                    // 右侧填充
                    for (int x = x_start; x <= x_end; x++) {
                        SetPixel(hdc, x, y, fillColor);
                    }
                } else {
                    // 跨越栅栏
                    for (int x = x_start; x <= fence_x; x++) {
                        SetPixel(hdc, x, y, fillColor);
                    }
                    for (int x = fence_x + 1; x <= x_end; x++) {
                        SetPixel(hdc, x, y, fillColor);
                    }
                }
            }
        }
    }
}

// 四连通洪泛填充
void FloodFill4(HDC hdc, int x, int y, COLORREF oldColor, COLORREF newColor) {
    if (oldColor == newColor) return;

    stack<Point> points;
    points.push(Point(x, y));

    while (!points.empty()) {
        Point p = points.top();
        points.pop();

        int currentX = p.x;
        int currentY = p.y;

        // 边界检查
        if (currentX < 0 || currentY < 0 || currentX >= 2000 || currentY >= 2000) continue;

        // 检查颜色
        if (GetPixel(hdc, currentX, currentY) != oldColor) continue;

        // 设置新颜色
        SetPixel(hdc, currentX, currentY, newColor);

        // 四方向填充
        points.push(Point(currentX + 1, currentY));
        points.push(Point(currentX - 1, currentY));
        points.push(Point(currentX, currentY + 1));
        points.push(Point(currentX, currentY - 1));
    }
}

// 八连通洪泛填充
void FloodFill8(HDC hdc, int x, int y, COLORREF oldColor, COLORREF newColor) {
    if (oldColor == newColor) return;

    stack<Point> points;
    points.push(Point(x, y));

    while (!points.empty()) {
        Point p = points.top();
        points.pop();

        int currentX = p.x;
        int currentY = p.y;

        // 边界检查
        if (currentX < 0 || currentY < 0 || currentX >= 2000 || currentY >= 2000) continue;

        // 检查颜色
        if (GetPixel(hdc, currentX, currentY) != oldColor) continue;

        // 设置新颜色
        SetPixel(hdc, currentX, currentY, newColor);

        // 八方向填充
        points.push(Point(currentX + 1, currentY));
        points.push(Point(currentX - 1, currentY));
        points.push(Point(currentX, currentY + 1));
        points.push(Point(currentX, currentY - 1));
        points.push(Point(currentX + 1, currentY + 1));
        points.push(Point(currentX + 1, currentY - 1));
        points.push(Point(currentX - 1, currentY + 1));
        points.push(Point(currentX - 1, currentY - 1));
    }
}

// 判断点是否在多边形内（射线法）
bool IsPointInPolygon(int x, int y, const std::vector<Point>& points) {
    if (points.size() < 3) return false;

    bool inside = false;
    for (size_t i = 0, j = points.size() - 1; i < points.size(); j = i++) {
        const Point& p1 = points[i];
        const Point& p2 = points[j];

        if (((p1.y > y) != (p2.y > y)) &&
            (x < (p2.x - p1.x) * (y - p1.y) / (p2.y - p1.y) + p1.x)) {
            inside = !inside;
        }
    }
    return inside;
}

// 查找包含指定点的封闭图形
std::vector<Point> FindClosedShapeAtPoint(const std::vector<GraphicShape>& shapes, int x, int y) {
    for (const auto& shape : shapes) {
        // 只检查可以形成封闭区域的图形类型
        if (shape.mode == MODE_RECTANGLE ||
            shape.mode == MODE_POLYLINE ||
            shape.mode == MODE_BSPLINE) {

            if (IsPointInPolygon(x, y, shape.points)) {
                return shape.points;
            }
        }
    }
    return std::vector<Point>(); // 返回空向量表示未找到
}

// 在指定点进行区域填充
bool FillAreaAtPoint(HDC hdc, int x, int y, DrawMode fillMode, COLORREF fillColor) {
    // 获取点击位置的颜色
    COLORREF targetColor = GetPixel(hdc, x, y);

    // 如果是白色背景，使用洪泛填充
    if (targetColor == RGB(255, 255, 255)) {
        switch (fillMode) {
            case MODE_FILL_SCANLINE:
                FloodFill4(hdc, x, y, targetColor, fillColor);
                break;
            case MODE_FILL_FENCE:
                FloodFill8(hdc, x, y, targetColor, fillColor);
                break;
            default:
                return false;
        }
        return true;
    }

    return false;
}