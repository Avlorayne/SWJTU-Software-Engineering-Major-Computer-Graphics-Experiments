#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <Windows.h>
#include "../../include/common.h"
#include "../models/GraphicsTypes.h"

// 全局变量声明
extern HWND g_hTabCtrl;
extern HWND g_hCurrentPage;
extern int g_currentTab;

// 选项卡管理函数
void CreateTabControl(HWND hWnd);
void SwitchTab(int tabIndex);
void HandleWindowResize(HWND hWnd);
LRESULT HandleNotifyMessage(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 页面窗口过程声明
LRESULT CALLBACK ExistingPageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InteractivePageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // TABMANAGER_H