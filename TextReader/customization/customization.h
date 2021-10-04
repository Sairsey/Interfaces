#ifndef __CUSTOMIZATION_H_
#define __CUSTOMIZATION_H_
#include "../defines.h"

// Font representation structure
typedef struct
{
    HFONT hFont; // my font
    unsigned long LineHeight; // size of font actually
    unsigned long BaselineToBaseline; // distance from baseline to baseline
    int SymbolWidth[256]; // Table that contains symbols width (for non-monospace fonts)
    unsigned long MinSymbolWidth;  // For determining maximum line width in characters
} font_params;

// structure with different customization_params
typedef struct
{
    font_params Font;
    COLORREF BackgroundColor;
    HBRUSH BackgroundBrush;
    COLORREF TextColor;
} customization_params;

// Initialize customization_params with default variables
// INPUT:
//   HWND hwnd - window handle for setting customization
// OUTPUT:
//   customization_params *custom - structure filled with default params
void CustomizationSetDefault(HWND hwnd, customization_params *custom);

// Clear customization_params with default variables
// INPUT:
//   customization_params *custom - structure filled with some params
// OUTPUT:
//   customization_params *custom - structure filled with zero data
void CustomizationClear(customization_params *custom);

// Count number of pixels this 0-terminated string will take with current font
// INPUT:
//   font_params *font - structure filled with font data
//   char *line - 0-terminated string
// OUTPUT:
//   unsigned long (returned) - Amount of pixels this string will take
unsigned long CustomizationGetLineWidth(font_params *font, char *line);

// Calculate number of formatted lines this line will take with client area width equal to window_width
// INPUT:
//   font_params *font - structure filled with font data
//   char *line - 0-terminated string
//   unsigned long window_width - size of window
// OUTPUT:
//   unsigned long (returned) - Amount of formatted lines
unsigned long CustomizationGetTextLineScreenLines(font_params *font, char *line, unsigned long window_width);


// Set font and fill metadata
// INPUT:
//   HWND hwnd - window handle for setting font
//   TCHAR *font_name - Name of font
// OUTPUT:
//   font_params *font - structure with correct metadata
void CustomizationSetFont(HWND hwnd, TCHAR *font_name, font_params *font);

#endif // __CUSTOMIZATION_H_
