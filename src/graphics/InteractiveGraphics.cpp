#include "InteractiveGraphics.h"
#include "../ui/UIComponents.h"
#include "DrawingAlgorithms.h"
#include "GraphicsManager.h"
#include "FillAlgorithms.h"
#include <cmath>

// 使用图形管理器
GraphicsManager& g_graphicsManager = GraphicsManager::GetInstance();

// 定义功能按钮的ID
#define ID_COMPLETE_SHAPE 100
#define ID_CANCEL_SHAPE   101
#define ID_CLEAR_ALL      102

void InitializeInteractivePage(HWND hWnd) {
    int buttonWidth = 120;
    int buttonHeight = 25;
    int startX = 10;
    int startY = 10;
    int spacing = 30;

    // 创建模式按钮
    AddModeButton(hWnd, startX, startY, buttonWidth, buttonHeight, L"直线-中点法", MODE_LINE_MIDPOINT);
    AddModeButton(hWnd, startX, startY + spacing, buttonWidth, buttonHeight, L"直线-Bresenham", MODE_LINE_BRESENHAM);
    AddModeButton(hWnd, startX, startY + spacing * 2, buttonWidth, buttonHeight, L"圆-中点法", MODE_CIRCLE_MIDPOINT);
    AddModeButton(hWnd, startX, startY + spacing * 3, buttonWidth, buttonHeight, L"圆-Bresenham", MODE_CIRCLE_BRESENHAM);
    AddModeButton(hWnd, startX, startY + spacing * 4, buttonWidth, buttonHeight, L"矩形", MODE_RECTANGLE);
    AddModeButton(hWnd, startX, startY + spacing * 5, buttonWidth, buttonHeight, L"多段线", MODE_POLYLINE);
    AddModeButton(hWnd, startX, startY + spacing * 6, buttonWidth, buttonHeight, L"B样条曲线", MODE_BSPLINE);
    AddModeButton(hWnd, startX, startY + spacing * 7, buttonWidth, buttonHeight, L"扫描线填充", MODE_FILL_SCANLINE);
    AddModeButton(hWnd, startX, startY + spacing * 8, buttonWidth, buttonHeight, L"栅栏填充", MODE_FILL_FENCE);

    // 添加功能按钮
    AddToolButton(hWnd, startX, startY + spacing * 9, buttonWidth, buttonHeight, L"完成图形", ID_COMPLETE_SHAPE);
    AddToolButton(hWnd, startX, startY + spacing * 10, buttonWidth, buttonHeight, L"取消绘制", ID_CANCEL_SHAPE);
    AddToolButton(hWnd, startX, startY + spacing * 11, buttonWidth, buttonHeight, L"清空所有", ID_CLEAR_ALL);
}

void DrawInteractiveGraphics(HDC hdc) {
    // 绘制说明文字
    TextOut(hdc, 150, 15, L"交互式图形绘制页面", 9);
    TextOut(hdc, 150, 40, L"左键点击绘制，右键结束绘制", 13);

    // 显示当前模式
    wchar_t modeText[100];
    switch (g_graphicsManager.GetCurrentMode()) {
    case MODE_LINE_MIDPOINT: wsprintf(modeText, L"当前模式: 直线-中点法"); break;
    case MODE_LINE_BRESENHAM: wsprintf(modeText, L"当前模式: 直线-Bresenham"); break;
    case MODE_CIRCLE_MIDPOINT: wsprintf(modeText, L"当前模式: 圆-中点法"); break;
    case MODE_CIRCLE_BRESENHAM: wsprintf(modeText, L"当前模式: 圆-Bresenham"); break;
    case MODE_RECTANGLE: wsprintf(modeText, L"当前模式: 矩形"); break;
    case MODE_POLYLINE: wsprintf(modeText, L"当前模式: 多段线"); break;
    case MODE_BSPLINE: wsprintf(modeText, L"当前模式: B样条曲线"); break;
    case MODE_FILL_SCANLINE: wsprintf(modeText, L"当前模式: 扫描线填充"); break;
    case MODE_FILL_FENCE: wsprintf(modeText, L"当前模式: 栅栏填充"); break;
    default: wsprintf(modeText, L"当前模式: 无"); break;
    }
    TextOut(hdc, 150, 65, modeText, wcslen(modeText));

    // 显示绘制状态和提示
    if (g_graphicsManager.IsDrawing()) {
        wchar_t statusText[100];
        wsprintf(statusText, L"正在绘制: %d个点", g_graphicsManager.GetCurrentPoints().size());
        TextOut(hdc, 150, 90, statusText, wcslen(statusText));

        // 各种模式的提示
        if (g_graphicsManager.GetCurrentMode() == MODE_BSPLINE) {
            TextOut(hdc, 150, 115, L"提示: B样条曲线需要至少4个控制点", 18);
        }
    } else if (g_graphicsManager.GetCurrentMode() == MODE_FILL_SCANLINE ||
               g_graphicsManager.GetCurrentMode() == MODE_FILL_FENCE) {
        TextOut(hdc, 150, 90, L"提示: 点击封闭区域内部进行填充", 15);
    }

    // 绘制所有已完成的图形（只绘制边界，不自动填充）
    for (const auto& shape : g_graphicsManager.GetAllShapes()) {
        if (shape.points.size() < 2) continue;

        HPEN hShapePen = CreatePen(PS_SOLID, 2, shape.color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hShapePen);

        switch (shape.mode) {
        case MODE_LINE_MIDPOINT:
            if (shape.points.size() == 2)
                DrawLineMidpoint(hdc, shape.points[0].x, shape.points[0].y,
                               shape.points[1].x, shape.points[1].y);
            break;

        case MODE_LINE_BRESENHAM:
            if (shape.points.size() == 2)
                DrawLineBresenham(hdc, shape.points[0].x, shape.points[0].y,
                                shape.points[1].x, shape.points[1].y);
            break;

        case MODE_CIRCLE_MIDPOINT:
            if (shape.points.size() == 2) {
                int dx = shape.points[1].x - shape.points[0].x;
                int dy = shape.points[1].y - shape.points[0].y;
                int radius = static_cast<int>(sqrt(dx * dx + dy * dy));
                DrawCircleMidpoint(hdc, shape.points[0].x, shape.points[0].y, radius);
            }
            break;

        case MODE_CIRCLE_BRESENHAM:
            if (shape.points.size() == 2) {
                int dx = shape.points[1].x - shape.points[0].x;
                int dy = shape.points[1].y - shape.points[0].y;
                int radius = static_cast<int>(sqrt(dx * dx + dy * dy));
                DrawCircleBresenham(hdc, shape.points[0].x, shape.points[0].y, radius);
            }
            break;

        case MODE_RECTANGLE:
            if (shape.points.size() == 2) {
                DrawRectangle(hdc, shape.points[0].x, shape.points[0].y,
                             shape.points[1].x, shape.points[1].y);
            }
            break;

        case MODE_POLYLINE:
            DrawPolyline(hdc, shape.points);
            // 如果是封闭多边形，连接首尾点
            if (shape.points.size() >= 3) {
                MoveToEx(hdc, shape.points.back().x, shape.points.back().y, NULL);
                LineTo(hdc, shape.points.front().x, shape.points.front().y);
            }
            break;

        case MODE_BSPLINE:
            DrawBSpline(hdc, shape.points);
            break;

        // 填充图形只存储边界点，不在这里绘制填充
        }

        SelectObject(hdc, hOldPen);
        DeleteObject(hShapePen);

        // 显示图形的控制点
        DrawControlPoints(hdc, shape.points, RGB(0, 0, 0));
    }

    // 绘制当前正在绘制的图形
    if (g_graphicsManager.IsDrawing()) {
        const auto& currentPoints = g_graphicsManager.GetCurrentPoints();
        if (currentPoints.size() >= 1) {
            HPEN hTempPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hTempPen);

            switch (g_graphicsManager.GetCurrentMode()) {
            case MODE_LINE_MIDPOINT:
            case MODE_LINE_BRESENHAM:
                if (currentPoints.size() == 2) {
                    if (g_graphicsManager.GetCurrentMode() == MODE_LINE_MIDPOINT)
                        DrawLineMidpoint(hdc, currentPoints[0].x, currentPoints[0].y,
                                       currentPoints[1].x, currentPoints[1].y);
                    else
                        DrawLineBresenham(hdc, currentPoints[0].x, currentPoints[0].y,
                                        currentPoints[1].x, currentPoints[1].y);
                } else if (currentPoints.size() == 1) {
                    DrawControlPoints(hdc, currentPoints, RGB(0, 0, 255));
                }
                break;

            case MODE_CIRCLE_MIDPOINT:
            case MODE_CIRCLE_BRESENHAM:
                if (currentPoints.size() == 2) {
                    int dx = currentPoints[1].x - currentPoints[0].x;
                    int dy = currentPoints[1].y - currentPoints[0].y;
                    int radius = static_cast<int>(sqrt(dx * dx + dy * dy));
                    if (g_graphicsManager.GetCurrentMode() == MODE_CIRCLE_MIDPOINT)
                        DrawCircleMidpoint(hdc, currentPoints[0].x, currentPoints[0].y, radius);
                    else
                        DrawCircleBresenham(hdc, currentPoints[0].x, currentPoints[0].y, radius);
                } else if (currentPoints.size() == 1) {
                    DrawControlPoints(hdc, currentPoints, RGB(0, 0, 255));
                }
                break;

            case MODE_RECTANGLE:
                if (currentPoints.size() == 2) {
                    DrawRectangle(hdc, currentPoints[0].x, currentPoints[0].y,
                                 currentPoints[1].x, currentPoints[1].y);
                } else if (currentPoints.size() == 1) {
                    DrawControlPoints(hdc, currentPoints, RGB(0, 0, 255));
                }
                break;

            case MODE_POLYLINE:
                if (currentPoints.size() >= 2) {
                    DrawPolyline(hdc, currentPoints);
                }
                DrawControlPoints(hdc, currentPoints, RGB(0, 0, 255));
                break;

            case MODE_BSPLINE:
                if (currentPoints.size() >= 4) {
                    DrawBSpline(hdc, currentPoints);
                } else if (currentPoints.size() >= 2) {
                    DrawPolyline(hdc, currentPoints);
                }
                DrawControlPoints(hdc, currentPoints, RGB(0, 0, 255));
                break;
            }

            SelectObject(hdc, hOldPen);
            DeleteObject(hTempPen);
        }
    }
}

LRESULT CALLBACK InteractivePageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 绘制白色背景
        DrawBackground(hdc, hWnd);

        DrawInteractiveGraphics(hdc);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        if (g_graphicsManager.GetCurrentMode() != MODE_NONE) {
            // 如果是填充模式，执行填充操作
            if (g_graphicsManager.GetCurrentMode() == MODE_FILL_SCANLINE ||
                g_graphicsManager.GetCurrentMode() == MODE_FILL_FENCE) {

                // 获取设备上下文用于填充
                HDC hdc = GetDC(hWnd);

                // 执行填充
                COLORREF fillColor = (g_graphicsManager.GetCurrentMode() == MODE_FILL_SCANLINE) ?
                                    RGB(0, 255, 0) : RGB(0, 0, 255);

                bool filled = FillAreaAtPoint(hdc, x, y, g_graphicsManager.GetCurrentMode(), fillColor);

                // 释放设备上下文
                ReleaseDC(hWnd, hdc);

                if (filled) {
                    // 填充成功
                    // 我们可以在这里添加成功提示，或者直接重绘
                }

                // 强制重绘以显示填充结果
                InvalidateRect(hWnd, nullptr, TRUE);
            }
            else {
                // 其他图形模式的正常绘制逻辑
                if (!g_graphicsManager.IsDrawing()) {
                    g_graphicsManager.StartNewShape(g_graphicsManager.GetCurrentMode());
                }
                g_graphicsManager.AddPoint(x, y);
                InvalidateRect(hWnd, nullptr, TRUE);
            }
        }
        break;
    }

    case WM_RBUTTONDOWN: {
        if (g_graphicsManager.IsDrawing()) {
            g_graphicsManager.CompleteShape();
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        break;
    }

    case WM_COMMAND: {
        int cmd = LOWORD(wParam);
        if (cmd >= MODE_LINE_MIDPOINT && cmd <= MODE_FILL_FENCE) {
            // 切换模式
            g_graphicsManager.SwitchMode((DrawMode)cmd);
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        else if (cmd == ID_COMPLETE_SHAPE) {
            g_graphicsManager.CompleteShape();
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        else if (cmd == ID_CANCEL_SHAPE) {
            g_graphicsManager.CancelShape();
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        else if (cmd == ID_CLEAR_ALL) {
            g_graphicsManager.ClearAllShapes();
            InvalidateRect(hWnd, nullptr, TRUE);
        }
        break;
    }

    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}