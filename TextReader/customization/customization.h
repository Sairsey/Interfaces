#ifndef __CUSTOMIZATION_H_
#define __CUSTOMIZATION_H_
#include "../defines.h"

typedef struct
{
    HFONT hFont; // my font
    unsigned long LineHeight; // size of font actually
    unsigned long BaselineToBaseline; // distance from baseline to baseline
    unsigned long SymbolWidth[256]; // Table that contains symbols width (for non-monospace fonts)
    unsigned long MinSymbolWidth;  // For determining maximum line width in characters
} font_params;

typedef struct
{
    font_params Font;
    COLORREF BackgroundColor;
    HBRUSH BackgroundBrush;
    COLORREF TextColor;
} customization_params;

void CustomisationSetDefault(HWND hwnd, customization_params *custom);

void CustomisationClear(customization_params *custom);

// returns number of pixels this 0-terminated line have with current font
unsigned long CustomisationGetLineWidth(font_params *font, char *line);

// returns number of formatted lines from default line
unsigned long CustomisationGetTextLineScreenLines(font_params *font, char *line, unsigned long window_width);

void CustomisationSetFont(HWND hwnd, TCHAR *font_name, font_params *font);

#endif // __CUSTOMIZATION_H_
