#include "customization.h"

// Initialize customization_params with default variables
// INPUT:
//   HWND hwnd - window handle for setting customization
// OUTPUT:
//   customization_params *custom - structure filled with default params
void CustomizationSetDefault(HWND hwnd, customization_params *custom)
{
    // just like VS2017
    custom->BackgroundColor = RGB(30, 30, 30);
    custom->TextColor = RGB(255, 255, 255);
    custom->Font.hFont = NULL;
    CustomizationSetFont(hwnd, "Monotype Corsiva", &custom->Font);

    // set background and text color
    {
        HDC hDC; // Drawable Context
        hDC = GetDC(hwnd);
        SetTextColor(hDC, custom->TextColor);
        custom->BackgroundBrush = CreateSolidBrush(custom->BackgroundColor);
        SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)custom->BackgroundBrush);
    }
}

// Clear customization_params with default variables
// INPUT:
//   customization_params *custom - structure filled with some params
// OUTPUT:
//   customization_params *custom - structure filled with zero data
void CustomizationClear(customization_params *custom)
{
    DeleteObject(custom->Font.hFont);
    DeleteObject(custom->BackgroundBrush);
}

// Count number of pixels this 0-terminated string will take with current font
// INPUT:
//   font_params *font - structure filled with font data
//   char *line - 0-terminated string
// OUTPUT:
//   unsigned long (returned) - Amount of pixels this string will take
unsigned long CustomizationGetLineWidth(font_params *font, char *line)
{
    unsigned long tmp = 0;
    for (; *line != 0; line++)
        tmp += font->SymbolWidth[(unsigned char)*line];
    return tmp;
}

// Calculate number of formatted lines this line will take with client area width equal to window_width
// INPUT:
//   font_params *font - structure filled with font data
//   char *line - 0-terminated string
//   unsigned long window_width - size of window
// OUTPUT:
//   unsigned long (returned) - Amount of formatted lines
unsigned long CustomizationGetTextLineScreenLines(font_params *font, char *line, unsigned long window_width)
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


// Set font and fill metadata
// INPUT:
//   HWND hwnd - window handle for setting font
//   TCHAR *font_name - Name of font
// OUTPUT:
//   font_params *font - structure with correct metadata
void CustomizationSetFont(HWND hwnd, TCHAR *font_name, font_params *font)
{
    if (font->hFont != NULL)
        DeleteObject(font->hFont);

    font->LineHeight = 50;

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
            GetCharWidth32(hDC, i, i, &(font->SymbolWidth[i]));

        font->MinSymbolWidth = font->SymbolWidth[0];

        for (i = 1; i < 256; i++)
            font->MinSymbolWidth = font->MinSymbolWidth < font->SymbolWidth[i] ? font->MinSymbolWidth : font->SymbolWidth[i];
        free(OutlineMetric);
    }
}
