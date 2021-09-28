#ifndef __SCREEN_BUFFER_
#define __SCREEN_BUFFER_
#include "../defines.h"
#include "../customization/customization.h"
#include "../input_buffer/input_buffer.h"


typedef enum
{
    FORMATTED = 0,  // No X-coord scrolling
    UNFORMATTED    // X-coord scrolling possible
} mode;

typedef struct
{
    char **Data;            // lines (as pointers in input_buffer->Data)
    unsigned long *Lengths; // length of every line
    unsigned long Size;     // number of lines
    unsigned long ScrollV;  // Scroll over Y coord
    unsigned long ScrollH;  // Scroll over W coord
    mode ViewMode;          // mode of view
    unsigned long WindowHeightInLines; // Height of window in lines;
    unsigned long WindowWidthInMinSymbols; // Width of window as number of symbols with min width, that could fit;
    unsigned long MaxLineLength; // Max length of lines.
} screen_buffer;

void ScreenBufferInit(screen_buffer *buffer);

void ScreenBufferBuild(input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height);

// resize buffer and apply all settings to scrolls
void ScreenBufferResize(HWND hwnd, input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height);

// Set exact position
void ScreenBufferSetVScroll(HWND hwnd, screen_buffer *out_buf, unsigned long pos);

// Set exact position
void ScreenBufferSetHScroll(HWND hwnd, screen_buffer *out_buf, unsigned long pos);

// Set position by delta (useful for Mousewheel and buttons)
void ScreenBufferChangeVScroll(HWND hwnd, screen_buffer *out_buf, long delta);

// Set position by delta (useful for Mousewheel and buttons)
void ScreenBufferChangeHScroll(HWND hwnd, screen_buffer *out_buf, long delta);

void ScreenBufferDraw(HWND hwnd, screen_buffer *buffer, font_params *font);

void ScreenBufferClear(screen_buffer *buffer);

#endif // __SCREEN_BUFFER_
