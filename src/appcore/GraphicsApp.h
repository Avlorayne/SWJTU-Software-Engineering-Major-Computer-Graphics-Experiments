#ifndef GRAPHICSAPP_H
#define GRAPHICSAPP_H

#include <Windows.h>
#include "../../include/common.h"

// 应用程序类
class GraphicsApp {
public:
    static GraphicsApp& GetInstance();
    bool Initialize(HINSTANCE hInstance);
    int Run();
    
    HINSTANCE GetInstanceHandle() const { return m_hInstance; }
    HWND GetMainWindow() const { return m_hWnd; }

private:
    GraphicsApp() = default;
    ~GraphicsApp() = default;
    
    bool RegisterWindowClass();
    bool CreateMainWindow();
    
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    HINSTANCE m_hInstance = nullptr;
    HWND m_hWnd = nullptr;
};

#endif // GRAPHICSAPP_H