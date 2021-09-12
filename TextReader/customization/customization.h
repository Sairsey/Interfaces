#ifndef __CUSTOMIZATION_H_
#define __CUSTOMIZATION_H_
#include "../defines.h"

typedef struct
{
    HFONT hFont; // my font
    long LineHeight; // size of font actually
    long BaselineToBaseline; // distance from baseline to baseline
    long SymbolWidth[256]; // Table that contains symbols width (for non-monospace fonts)
    long MinSymbolWidth;  // For determining maximum line width in characters
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

void CustomisationSetFont(HWND hwnd, TCHAR *font_name, font_params *font);

#endif // __CUSTOMIZATION_H_
