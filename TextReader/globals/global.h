/* File with common used types/functions/etc. */
#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include <tchar.h>
#include <windows.h>

// flag for enabling logging
#define MY_DEBUG

#define TRUE 1
#define FALSE 0

#define TRU TRUE
#define FOLS FALSE

typedef int bool;

typedef struct tag_global_data
{
    bool IsInited;         // is system inited

    char *LoadedBuffer;    // pointer on loaded data
    long LoadedBufferSize; // size of loaded buffer

    char **DrawBuffer;     // pointer on data to draw
    long DrawBufferLines;  // amount of lines in array

    long WindowWidth;      // Width of window
    long WindowHeight;     // Width of window
    HWND HWnd;             // Handle Window
    HINSTANCE hInstanse;   // Handle Instance

    // Background color
    char BackRed;
    char BackBlue;
    char BackGreen;

    // Text color
    char TextRed;
    char TextBlue;
    char TextGreen;

    HFONT hFont; // my font
    long LineHeight; // size of font actually
    long BaselineToBaseline; // distance from baseline to baseline
    long SymbolWidth[256]; // Table that contains symbols width (for non-monospace fonts)
    long MinSymbolWidth;  // For determining maximum line width in characters


} global_data;

void MyDebugMessage(const char *format, ...);

#define SafeDelete(x) if ((x) != NULL) free((x));

#endif // __GLOBALS_H_
