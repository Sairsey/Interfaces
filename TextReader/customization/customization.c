#include "customization.h"

void CustomisationSetDefault(HWND hwnd, customization_params *custom)
{
    // just like VS2017
    custom->BackgroundColor = RGB(30, 30, 30);
    custom->TextColor = RGB(255, 255, 255);
    custom->Font.hFont = NULL;
    CustomisationSetFont(hwnd, "Times New Roman", &custom->Font);

    // set background and text color
    {
        HDC hDC; // Drawable Context
        hDC = GetDC(hwnd);
        SetTextColor(hDC, custom->TextColor);
        custom->BackgroundBrush = CreateSolidBrush(custom->BackgroundColor);
        SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)custom->BackgroundBrush);
    }
}

void CustomisationClear(customization_params *custom)
{
    DeleteObject(custom->Font.hFont);
    DeleteObject(custom->BackgroundBrush);
}

unsigned long CustomisationGetLineWidth(font_params *font, char *line)
{
    unsigned long tmp = 0;
    for (; *line != 0; line++)
        tmp += font->SymbolWidth[(unsigned char)*line];
    return tmp;
}

unsigned long CustomisationGetTextLineScreenLines(font_params *font, char *line, unsigned long window_width)
{
    unsigned long tmp = 1;
    unsigned long current_width = 0;
    for (; *line != 0; line++)
    {
        if (current_width + font->SymbolWidth[(unsigned char)*line] < window_width)
        {
            current_width += font->SymbolWidth[(unsigned char)*line];
        }
        else
        {
            current_width = font->SymbolWidth[(unsigned char)*line];
            tmp++;
        }
    }
    return tmp;
}


void CustomisationSetFont(HWND hwnd, TCHAR *font_name, font_params *font)
{
    if (font->hFont == NULL)
    {
        DeleteObject(font->hFont);
    }
    font->LineHeight = 32;

    // Create font I like
    font->hFont = CreateFont(font->LineHeight, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, font_name);

    // fill data
    {
        HDC hDC; // Drawable Context
        long OutlineMetricSize;
        OUTLINETEXTMETRIC *OutlineMetric;
        int i;

        hDC = GetDC(hwnd);
        // set font
        SelectObject(hDC, font->hFont);
        // Draw with transparent
        SetBkMode(hDC, TRANSPARENT);

        OutlineMetricSize = GetOutlineTextMetrics(hDC, 0, NULL);
        OutlineMetric = malloc(OutlineMetricSize);

        GetOutlineTextMetrics(hDC, OutlineMetricSize, OutlineMetric);

        font->BaselineToBaseline = OutlineMetric->otmMacAscent + OutlineMetric->otmMacDescent + OutlineMetric->otmMacLineGap + 10;

        for (i = 0; i < 256; i++)
        {
            GetCharWidth32(hDC, i, i, &(font->SymbolWidth[i]));
        }

        font->MinSymbolWidth = font->SymbolWidth[0];

        for (i = 1; i < 256; i++)
            font->MinSymbolWidth = font->MinSymbolWidth < font->SymbolWidth[i] ? font->MinSymbolWidth : font->SymbolWidth[i];
        free(OutlineMetric);
    }
}
