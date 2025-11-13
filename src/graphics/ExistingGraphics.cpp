#include "ExistingGraphics.h"

// 绘制原有工程图
void DrawExistingGraphics(HDC hdc) {
    // 定义基准点和缩放因子
    int centerX = 400;
    int centerY = 300;
    float scale = 6.0f;

    // 创建画笔
    HPEN hCenterLinePen = CreatePen(PS_DOT, 1, RGB(128, 128, 128));
    HPEN hOutlinePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN hDimensionPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    HPEN hHolePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

    HPEN hOldPen = (HPEN)SelectObject(hdc, hCenterLinePen);

    // 缩放函数
    auto S = [scale](float value) -> int {
        return static_cast<int>(value * scale + 0.5f);
    };

    // 绘制中心线
    MoveToEx(hdc, centerX - S(50), centerY, NULL);
    LineTo(hdc, centerX + S(50), centerY);
    MoveToEx(hdc, centerX, centerY - S(40), NULL);
    LineTo(hdc, centerX, centerY + S(40));

    // 绘制外轮廓
    SelectObject(hdc, hOutlinePen);
    int outerLeft = centerX - S(33);
    int outerTop = centerY - S(23);
    int outerRight = centerX + S(33);
    int outerBottom = centerY + S(23);

    // 绘制外轮廓直线部分
    MoveToEx(hdc, outerLeft + S(7), outerTop, NULL);
    LineTo(hdc, outerRight - S(7), outerTop);
    MoveToEx(hdc, outerLeft + S(7), outerBottom, NULL);
    LineTo(hdc, outerRight - S(7), outerBottom);
    MoveToEx(hdc, outerLeft, outerTop + S(7), NULL);
    LineTo(hdc, outerLeft, outerBottom - S(7));
    MoveToEx(hdc, outerRight, outerTop + S(7), NULL);
    LineTo(hdc, outerRight, outerBottom - S(7));

    // 绘制圆角
    Arc(hdc, outerLeft, outerTop, outerLeft + S(14), outerTop + S(14),
        outerLeft + S(7), outerTop, outerLeft, outerTop + S(7));
    Arc(hdc, outerRight - S(14), outerTop, outerRight, outerTop + S(14),
        outerRight, outerTop + S(7), outerRight - S(7), outerTop);
    Arc(hdc, outerLeft, outerBottom - S(14), outerLeft + S(14), outerBottom,
        outerLeft, outerBottom - S(7), outerLeft + S(7), outerBottom);
    Arc(hdc, outerRight - S(14), outerBottom - S(14), outerRight, outerBottom,
        outerRight - S(7), outerBottom, outerRight, outerBottom - S(7));

    // 绘制内轮廓
    SelectObject(hdc, hOutlinePen);
    int innerLeft = centerX - S(21.5);
    int innerTop = centerY - S(15);
    int innerRight = centerX + S(21.5);
    int innerBottom = centerY + S(15);

    // 绘制内轮廓直线部分
    MoveToEx(hdc, innerLeft + S(3), innerTop, NULL);
    LineTo(hdc, innerRight - S(3), innerTop);
    MoveToEx(hdc, innerLeft + S(3), innerBottom, NULL);
    LineTo(hdc, innerRight - S(3), innerBottom);
    MoveToEx(hdc, innerLeft, innerTop + S(3), NULL);
    LineTo(hdc, innerLeft, innerBottom - S(3));
    MoveToEx(hdc, innerRight, innerTop + S(3), NULL);
    LineTo(hdc, innerRight, innerBottom - S(3));

    // 绘制内轮廓圆角
    Arc(hdc, innerLeft, innerTop, innerLeft + S(6), innerTop + S(6),
        innerLeft + S(3), innerTop, innerLeft, innerTop + S(3));
    Arc(hdc, innerRight - S(6), innerTop, innerRight, innerTop + S(6),
        innerRight, innerTop + S(3), innerRight - S(3), innerTop);
    Arc(hdc, innerLeft, innerBottom - S(6), innerLeft + S(6), innerBottom,
        innerLeft, innerBottom - S(3), innerLeft + S(3), innerBottom);
    Arc(hdc, innerRight - S(6), innerBottom - S(6), innerRight, innerBottom,
        innerRight - S(3), innerBottom, innerRight, innerBottom - S(3));

    // 绘制4个Φ7圆孔
    SelectObject(hdc, hHolePen);
    int holeHorizontalOffset = S(26);
    int holeVerticalOffset = S(16);

    // 左上圆孔
    Ellipse(hdc,
        centerX - holeHorizontalOffset - S(3.5), centerY - holeVerticalOffset - S(3.5),
        centerX - holeHorizontalOffset + S(3.5), centerY - holeVerticalOffset + S(3.5)
    );

    // 右上圆孔
    Ellipse(hdc,
        centerX + holeHorizontalOffset - S(3.5), centerY - holeVerticalOffset - S(3.5),
        centerX + holeHorizontalOffset + S(3.5), centerY - holeVerticalOffset + S(3.5)
    );

    // 左下圆孔
    Ellipse(hdc,
        centerX - holeHorizontalOffset - S(3.5), centerY + holeVerticalOffset - S(3.5),
        centerX - holeHorizontalOffset + S(3.5), centerY + holeVerticalOffset + S(3.5)
    );

    // 右下圆孔
    Ellipse(hdc,
        centerX + holeHorizontalOffset - S(3.5), centerY + holeVerticalOffset - S(3.5),
        centerX + holeHorizontalOffset + S(3.5), centerY + holeVerticalOffset + S(3.5)
    );

    // 绘制尺寸标注
    SelectObject(hdc, hDimensionPen);
    SetBkMode(hdc, TRANSPARENT);

    // 外部总长标注 66
    DrawDimensionLine(hdc, outerLeft, outerTop - S(15), outerRight, outerTop - S(15), L"66");

    // 外部总高标注 46
    DrawDimensionLine(hdc, outerRight + S(15), outerTop, outerRight + S(15), outerBottom, L"46");

    // 内轮廓长度标注 43
    DrawDimensionLine(hdc, innerLeft, innerBottom + S(10), innerRight, innerBottom + S(10), L"43");

    // 内轮廓高度标注 30
    DrawDimensionLine(hdc, innerRight + S(10), innerTop, innerRight + S(10), innerBottom, L"30");

    // 圆孔横向中心距标注 52
    DrawDimensionLine(hdc,
        centerX - holeHorizontalOffset, outerTop - S(25),
        centerX + holeHorizontalOffset, outerTop - S(25),
        L"52"
    );

    // 圆孔纵向中心距标注 32
    DrawDimensionLine(hdc,
        outerRight + S(25), centerY - holeVerticalOffset,
        outerRight + S(25), centerY + holeVerticalOffset,
        L"32"
    );

    // 圆角半径标注
    TextOut(hdc, outerLeft + S(5), outerTop + S(5), L"R7", 2);
    TextOut(hdc, innerLeft + S(2), innerTop + S(2), L"R3", 2);

    // 孔特征标注
    TextOut(hdc, centerX - S(15), centerY - S(25), L"4-Φ7", 5);

    // 绘制文本信息
    TextOut(hdc, 50, 50, L"何欣航 2023112560", 14);
    TextOut(hdc, 50, 80, L"工程图显示页面", 7);

    // 恢复原来的画笔并删除创建的画笔
    SelectObject(hdc, hOldPen);
    DeleteObject(hCenterLinePen);
    DeleteObject(hOutlinePen);
    DeleteObject(hDimensionPen);
    DeleteObject(hHolePen);
}

// 尺寸标注函数
void DrawDimensionLine(HDC hdc, int x1, int y1, int x2, int y2, LPCWSTR text) {
    HPEN hOldPen = (HPEN)GetCurrentObject(hdc, OBJ_PEN);
    HPEN hDimPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    SelectObject(hdc, hDimPen);

    // 绘制尺寸线
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);

    // 绘制简单箭头
    if (x1 == x2) { // 垂直尺寸线
        MoveToEx(hdc, x1 - 3, y1, NULL); LineTo(hdc, x1 + 3, y1);
        MoveToEx(hdc, x1 - 3, y2, NULL); LineTo(hdc, x1 + 3, y2);
    } else { // 水平尺寸线
        MoveToEx(hdc, x1, y1 - 3, NULL); LineTo(hdc, x1, y1 + 3);
        MoveToEx(hdc, x2, y2 - 3, NULL); LineTo(hdc, x2, y2 + 3);
    }

    // 添加尺寸文本
    int textX = (x1 + x2) / 2 - 10;
    int textY = (y1 + y2) / 2 - 8;
    TextOut(hdc, textX, textY, text, wcslen(text));

    SelectObject(hdc, hOldPen);
    DeleteObject(hDimPen);
}