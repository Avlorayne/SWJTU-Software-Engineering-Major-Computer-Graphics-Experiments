#include "GraphicsApp.h"
#include "../ui/TabManager.h"

GraphicsApp& GraphicsApp::GetInstance() {
    static GraphicsApp instance;
    return instance;
}

bool GraphicsApp::Initialize(HINSTANCE hInstance) {
    m_hInstance = hInstance;

    // 初始化通用控件
    InitializeCommonControls();

    // 注册窗口类
    if (!RegisterWindowClass()) {
        return false;
    }

    // 创建主窗口
    if (!CreateMainWindow()) {
        return false;
    }

    return true;
}

bool GraphicsApp::RegisterWindowClass() {
    const wchar_t CLASS_NAME[] = L"GraphicsWindow";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    return RegisterClassW(&wc) != 0;
}

bool GraphicsApp::CreateMainWindow() {
    m_hWnd = CreateWindowW(
        L"GraphicsWindow",
        L"图形绘制软件 - 何欣航 2023112560",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700,
        nullptr, nullptr, m_hInstance, nullptr
    );

    return m_hWnd != nullptr;
}

int GraphicsApp::Run() {
    if (!m_hWnd) {
        return 0;
    }

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    // 消息循环
    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}

LRESULT CALLBACK GraphicsApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            CreateTabControl(hWnd);
        SwitchTab(ID_EXISTING_PAGE);
        break;

        case WM_SIZE:
            HandleWindowResize(hWnd);
        break;

        case WM_NOTIFY:
            return HandleNotifyMessage(hWnd, wParam, lParam);

        case WM_DESTROY:
            PostQuitMessage(0);
        break;

        default:
            return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}