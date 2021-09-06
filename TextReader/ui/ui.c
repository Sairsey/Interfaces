
#include "ui.h"


void DrawTextOnScreen(global_data *globals)
{
    HDC hDC; // Drawable Context
    RECT ScreenRect;  // Client rect
    PAINTSTRUCT PaintStructure; // Paint structure
    HBRUSH BackgroundBrush;
    HFONT hfOld;                // Default font
    int i;

    if (globals->DrawBuffer == NULL)
    {
        MyDebugMessage("Somebody once... Run DrawTextOnScreen without DrawBuffer\n");
        return;
    }

    // Get client area size
    GetClientRect(globals->HWnd, &ScreenRect);

    // Get context for drawing
    hDC = BeginPaint(globals->HWnd, &PaintStructure);

    // Draw with transparent
    SetBkMode(hDC, OPAQUE);

    // create brush for background
    BackgroundBrush = CreateSolidBrush(RGB(globals->BackRed, globals->BackGreen, globals->BackBlue));

    // fill screen with bgcolor
    FillRect(hDC, &ScreenRect, BackgroundBrush);

    // Draw with transparent
    SetBkMode(hDC, TRANSPARENT);

    // set color and font
    SetTextColor(hDC, RGB(globals->TextRed, globals->TextGreen, globals->TextBlue));
    hfOld = SelectObject(hDC, globals->hFont);

    // Send some text on screen

    for (int i = 0; i < globals->DrawBufferLines; i++)
    {
        int len = strlen(globals->DrawBuffer[i]);
        TextOut(hDC, ScreenRect.left, ScreenRect.top + i * globals->BaselineToBaseline, globals->DrawBuffer[i], len);
    }


    SelectObject(hDC, hfOld);
    EndPaint(globals->HWnd, &PaintStructure);
}

void FillFontData(global_data *globals)
{
    HDC hDC; // Drawable Context
    PAINTSTRUCT PaintStructure; // Paint structure
    HFONT hfOld;                // Default font
    long OutlineMetricSize;
    OUTLINETEXTMETRIC *OutlineMetric;
    TEXTMETRIC TextMetric;
    int i;

    // Get context for drawing
    hDC = BeginPaint(globals->HWnd, &PaintStructure);
    hfOld = SelectObject(hDC, globals->hFont);

    OutlineMetricSize = GetOutlineTextMetrics(hDC, 0, NULL);
    OutlineMetric = malloc(OutlineMetricSize);

    GetOutlineTextMetrics(hDC, OutlineMetricSize, OutlineMetric);

    globals->BaselineToBaseline = OutlineMetric->otmMacAscent + OutlineMetric->otmMacDescent + OutlineMetric->otmMacLineGap + 10;

    for (i = 0; i < 256; i++)
    {
        GetCharWidth32(hDC, i, i, globals->SymbolWidth + i);
    }

    globals->MinSymbolWidth = globals->SymbolWidth[0];

    for (i = 1; i < 256; i++)
    {
        globals->MinSymbolWidth = globals->MinSymbolWidth < globals->SymbolWidth[i] ? globals->MinSymbolWidth : globals->SymbolWidth[i];
    }

    free(OutlineMetric);
    SelectObject(hDC, hfOld);
    EndPaint(globals->HWnd, &PaintStructure);
}
