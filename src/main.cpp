#include <Windows.h>
#include "../include/common.h"
#include "appcore/GraphicsApp.h"  // 修正路径

// 实现通用函数
void InitializeCommonControls() {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);
}

#ifdef __MINGW32__
int main() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
#endif
    // 获取应用程序实例并初始化
    GraphicsApp& app = GraphicsApp::GetInstance();

    if (!app.Initialize(hInstance)) {
        return 0;
    }

    // 运行应用程序
    return app.Run();
}