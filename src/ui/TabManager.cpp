#include "TabManager.h"
#include "../graphics/ExistingGraphics.h"
#include "../graphics/InteractiveGraphics.h"

// 全局变量定义
HWND g_hTabCtrl;
HWND g_hCurrentPage;
int g_currentTab = ID_EXISTING_PAGE;

void CreateTabControl(HWND hWnd) {
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    g_hTabCtrl = CreateWindowW(WC_TABCONTROLW, L"",
                              WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
                              0, 0, rcClient.right, rcClient.bottom,
                              hWnd, (HMENU)ID_TABCTRL, GetModuleHandle(nullptr), nullptr);

    // 添加选项卡
    TCITEMW tie = {};
    tie.mask = TCIF_TEXT;

    wchar_t tab1Text[] = L"工程图显示";
    wchar_t tab2Text[] = L"交互式图形绘制";

    tie.pszText = tab1Text;
    TabCtrl_InsertItem(g_hTabCtrl, ID_EXISTING_PAGE, &tie);

    tie.pszText = tab2Text;
    TabCtrl_InsertItem(g_hTabCtrl, ID_INTERACTIVE_PAGE, &tie);
}

void SwitchTab(int tabIndex) {
    if (g_hCurrentPage) {
        DestroyWindow(g_hCurrentPage);
        g_hCurrentPage = nullptr;
    }

    RECT rcTab;
    GetClientRect(g_hTabCtrl, &rcTab);
    TabCtrl_AdjustRect(g_hTabCtrl, FALSE, &rcTab);

    HINSTANCE hInstance = GetModuleHandle(nullptr);

    if (tabIndex == ID_EXISTING_PAGE) {
        g_hCurrentPage = CreateWindowW(L"Static", L"",
                                      WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
                                      rcTab.left, rcTab.top,
                                      rcTab.right - rcTab.left,
                                      rcTab.bottom - rcTab.top,
                                      g_hTabCtrl, (HMENU)1, hInstance, nullptr);

        SetWindowLongPtr(g_hCurrentPage, GWLP_WNDPROC, (LONG_PTR)ExistingPageProc);
    }
    else if (tabIndex == ID_INTERACTIVE_PAGE) {
        g_hCurrentPage = CreateWindowW(L"Static", L"",
                                      WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
                                      rcTab.left, rcTab.top,
                                      rcTab.right - rcTab.left,
                                      rcTab.bottom - rcTab.top,
                                      g_hTabCtrl, (HMENU)2, hInstance, nullptr);

        SetWindowLongPtr(g_hCurrentPage, GWLP_WNDPROC, (LONG_PTR)InteractivePageProc);
        InitializeInteractivePage(g_hCurrentPage);
    }

    g_currentTab = tabIndex;
    InvalidateRect(g_hCurrentPage, nullptr, TRUE);
}

void HandleWindowResize(HWND hWnd) {
    if (g_hTabCtrl) {
        RECT rcClient;
        GetClientRect(hWnd, &rcClient);
        SetWindowPos(g_hTabCtrl, nullptr, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);

        if (g_hCurrentPage) {
            RECT rcTab;
            GetClientRect(g_hTabCtrl, &rcTab);
            TabCtrl_AdjustRect(g_hTabCtrl, FALSE, &rcTab);
            SetWindowPos(g_hCurrentPage, nullptr, rcTab.left, rcTab.top,
                        rcTab.right - rcTab.left, rcTab.bottom - rcTab.top, SWP_NOZORDER);
        }
    }
}

LRESULT HandleNotifyMessage(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    NMHDR* pNMHDR = (NMHDR*)lParam;
    if (pNMHDR->idFrom == ID_TABCTRL && pNMHDR->code == TCN_SELCHANGE) {
        int selectedTab = TabCtrl_GetCurSel(g_hTabCtrl);
        SwitchTab(selectedTab);
    }
    return 0;
}

// 原有工程图页面窗口过程
LRESULT CALLBACK ExistingPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        
        // 绘制白色背景
        RECT rect;
        GetClientRect(hWnd, &rect);
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
        
        DrawExistingGraphics(hdc);
        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}