#include "GraphicsManager.h"

GraphicsManager& GraphicsManager::GetInstance() {
    static GraphicsManager instance;
    return instance;
}

void GraphicsManager::StartNewShape(DrawMode mode) {
    m_currentMode = mode;
    m_currentPoints.clear();
    m_isDrawing = true;
}

void GraphicsManager::AddPoint(int x, int y) {
    if (!m_isDrawing) return;

    m_currentPoints.push_back(Point(x, y));

    // 自动完成需要固定点数的图形
    if ((m_currentMode == MODE_LINE_MIDPOINT ||
         m_currentMode == MODE_LINE_BRESENHAM ||
         m_currentMode == MODE_RECTANGLE ||
         m_currentMode == MODE_CIRCLE_MIDPOINT ||
         m_currentMode == MODE_CIRCLE_BRESENHAM) &&
        m_currentPoints.size() == 2) {
        CompleteShape();
        }
}

void GraphicsManager::CompleteShape() {
    if (m_isDrawing && !m_currentPoints.empty()) {
        // 多段线需要至少2个点，其他图形需要满足各自条件
        if ((m_currentMode == MODE_POLYLINE && m_currentPoints.size() >= 2) ||
            (m_currentMode != MODE_POLYLINE && m_currentPoints.size() >= 1)) {
            m_shapes.push_back(GraphicShape(m_currentMode, m_currentPoints, RGB(255, 0, 0)));
            }
        m_currentPoints.clear();
        m_isDrawing = false;
    }
}

void GraphicsManager::CancelShape() {
    m_currentPoints.clear();
    m_isDrawing = false;
}

void GraphicsManager::ClearAllShapes() {
    m_shapes.clear();
    m_currentPoints.clear();
    m_isDrawing = false;
}

void GraphicsManager::SwitchMode(DrawMode newMode) {
    // 切换模式时取消当前绘制
    CancelShape();
    m_currentMode = newMode;
}