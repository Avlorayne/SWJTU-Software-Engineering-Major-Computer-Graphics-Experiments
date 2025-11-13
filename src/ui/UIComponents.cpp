#include "UIComponents.h"

// 添加普通按钮（使用整数ID）
void AddToolButton(HWND hWnd, int x, int y, int width, int height, const wchar_t* text, int id) {
    HWND hButton = CreateWindowW(L"BUTTON", text,
                                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                x, y, width, height,
                                hWnd, (HMENU)id, GetModuleHandle(nullptr), nullptr);
}

// 添加模式按钮（使用DrawMode枚举）
void AddModeButton(HWND hWnd, int x, int y, int width, int height, const wchar_t* text, DrawMode mode) {
    HWND hButton = CreateWindowW(L"BUTTON", text, 
                                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                x, y, width, height,
                                hWnd, (HMENU)mode, GetModuleHandle(nullptr), nullptr);
}

// 绘制控制点
void DrawControlPoints(HDC hdc, const std::vector<Point>& points, COLORREF color) {
    HPEN hPointPen = CreatePen(PS_SOLID, 1, color);
    HBRUSH hPointBrush = CreateSolidBrush(color);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPointPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hPointBrush);
    
    for (const auto& point : points) {
        Ellipse(hdc, point.x - 3, point.y - 3, point.x + 3, point.y + 3);
    }
    
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPointPen);
    DeleteObject(hPointBrush);
}

// 绘制背景
void DrawBackground(HDC hdc, HWND hWnd, COLORREF color) {
    RECT rect;
    GetClientRect(hWnd, &rect);
    HBRUSH hBrush = CreateSolidBrush(color);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
}