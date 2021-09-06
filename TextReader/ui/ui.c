
#include "ui.h"


void DrawTextOnScreen(global_data *globals)
{
    HDC hDC; // Drawable Context
    RECT ScreenRect;  // Client rect
    PAINTSTRUCT PaintStructure; // Paint structure
    HBRUSH BackgroundBrush;
    HFONT hfOld;                // Default font

    // Get client area size
    GetClientRect(globals->HWnd, &ScreenRect);

    // Get context for drawing
    hDC = BeginPaint(globals->HWnd, &PaintStructure);

    // create brush for background
    BackgroundBrush = CreateSolidBrush(RGB(globals->BackRed, globals->BackGreen, globals->BackBlue));

    // fill screen with bgcolor
    FillRect(hDC, &ScreenRect, BackgroundBrush);

    // Draw with transparent
    SetBkMode(hDC, TRANSPARENT);

    // set color and font
    SetTextColor(hDC, RGB(globals->TextRed, globals->TextGreen, globals->TextBlue));
    hfOld = SelectObject(hDC, globals->hFont);

    // Send some text
    TCHAR text[] = "Defenestration\n can be hazardous";
    TextOut(hDC, ScreenRect.left, ScreenRect.top, text, ARRAYSIZE(text));

    SelectObject(hDC, hfOld);
    EndPaint(globals->HWnd, &PaintStructure);
}
