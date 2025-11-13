#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <Windows.h>
#include <vector>
#include "../models/GraphicsTypes.h"

class GraphicsManager {
public:
    static GraphicsManager& GetInstance();
    
    // 图形管理
    void StartNewShape(DrawMode mode);
    void AddPoint(int x, int y);
    void CompleteShape();
    void CancelShape();
    void ClearAllShapes();
    
    // 状态查询
    bool IsDrawing() const { return m_isDrawing; }
    DrawMode GetCurrentMode() const { return m_currentMode; }
    const std::vector<Point>& GetCurrentPoints() const { return m_currentPoints; }
    const std::vector<GraphicShape>& GetAllShapes() const { return m_shapes; }
    
    // 模式切换
    void SwitchMode(DrawMode newMode);

private:
    GraphicsManager() = default;
    
    DrawMode m_currentMode = MODE_NONE;
    std::vector<Point> m_currentPoints;
    std::vector<GraphicShape> m_shapes;
    bool m_isDrawing = false;
};

#endif // GRAPHICSMANAGER_H