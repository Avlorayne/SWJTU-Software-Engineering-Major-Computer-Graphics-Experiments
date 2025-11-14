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
        // 多段线需要至少2个点
        if (m_currentMode == MODE_POLYLINE) {
            if (m_currentPoints.size() >= 2) {
                m_shapes.push_back(GraphicShape(m_currentMode, m_currentPoints, RGB(255, 0, 0)));
            }
        }
        // B样条曲线需要至少4个点
        else if (m_currentMode == MODE_BSPLINE) {
            if (m_currentPoints.size() >= 4) {
                m_shapes.push_back(GraphicShape(m_currentMode, m_currentPoints, RGB(255, 0, 0)));
            }
        }
        // 填充图形需要至少3个点（形成封闭区域）
        else if (m_currentMode == MODE_FILL_SCANLINE || m_currentMode == MODE_FILL_FENCE) {
            if (m_currentPoints.size() >= 3) {
                m_shapes.push_back(GraphicShape(m_currentMode, m_currentPoints, RGB(0, 255, 0))); // 使用绿色表示填充图形
            }
        }
        // 对于其他图形，需要满足各自的最小点数要求
        else if (m_currentMode != MODE_POLYLINE && m_currentMode != MODE_BSPLINE &&
                 m_currentMode != MODE_FILL_SCANLINE && m_currentMode != MODE_FILL_FENCE) {
            if ((m_currentMode == MODE_LINE_MIDPOINT ||
                 m_currentMode == MODE_LINE_BRESENHAM ||
                 m_currentMode == MODE_RECTANGLE ||
                 m_currentMode == MODE_CIRCLE_MIDPOINT ||
                 m_currentMode == MODE_CIRCLE_BRESENHAM) &&
                m_currentPoints.size() == 2) {
                m_shapes.push_back(GraphicShape(m_currentMode, m_currentPoints, RGB(255, 0, 0)));
                }
                 }

        // 无论是否保存，都清空当前点并结束绘制状态
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
    // 如果正在绘制，先完成当前图形
    if (m_isDrawing && !m_currentPoints.empty()) {
        CompleteShape();
    }

    // 切换模式并重置状态
    m_currentMode = newMode;
    m_currentPoints.clear();
    m_isDrawing = false;
}