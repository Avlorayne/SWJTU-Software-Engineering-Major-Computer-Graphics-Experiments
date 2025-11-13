#ifndef COMMON_H
#define COMMON_H

#include <Windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

// 通用定义和常量
#define ID_TABCTRL 1001
#define ID_EXISTING_PAGE 0
#define ID_INTERACTIVE_PAGE 1

// 通用函数声明
void InitializeCommonControls();

#endif // COMMON_H