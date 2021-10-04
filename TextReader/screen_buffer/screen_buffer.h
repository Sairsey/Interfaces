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
    unsigned long Size;     // number of lines
    unsigned long ScrollV;  // Scroll over Y coord
    unsigned long ScrollH;  // Scroll over X coord
    mode ViewMode;          // mode of view
    unsigned long WindowHeightInLines; // Height of window in lines;
    unsigned long WindowWidthInMinSymbols; // Width of window as number of symbols with min width, that could fit;
    unsigned long MaxLineLength; // Max length of lines.
} screen_buffer;

// Initialize screen buffer
// INPUT:
//   screen_buffer *buffer - pointer on screen_buffer structure
// OUTPUT:
//   screen_buffer *buffer - pointer on screen_buffer structure filled with zero values
void ScreenBufferInit(screen_buffer *buffer);

// Build screen buffer
// INPUT:
//   input_buffer *in_buf - structure with data and metadata from file.
//   screen_buffer *out_buf - structure with right ViewMode
//   font_params *font - structure with metadata about current font (width of characters, height and so on)
//   long window_width, - size of window client area
//   long window_height - size of window client area
// OUTPUT:
//   screen_buffer *out_buf - structure filled with correct data and ready do be used in ScreenBufferDraw
void ScreenBufferBuild(input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height);

// Rebuild out_buf and apply all settings to scrolls
// INPUT:
//   HWND hwnd - window handle for setting scrolling positions
//   input_buffer *in_buf - structure with data and metadata from file.
//   screen_buffer *out_buf - structure with right ViewMode and scrolls data
//   font_params *font - structure with metadata about current font (width of characters, height and so on)
//   long window_width, - size of window client area
//   long window_height - size of window client area
// OUTPUT:
//   screen_buffer *out_buf - structure filled with correct data and ready do be used in ScreenBufferDraw
void ScreenBufferResize(HWND hwnd, input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height);

// Set position of Vertical Scroll with absolute coordinate
// INPUT:
//   HWND hwnd - window handle for setting scrolling positions
//   screen_buffer *out_buf - structure with right ViewMode and scrolls data
//   long pos - position of scroll
// OUTPUT:
//   screen_buffer *out_buf - structure with correctly modified Scroll info
void ScreenBufferSetVScroll(HWND hwnd, screen_buffer *out_buf, unsigned long pos);

// Set position of Horisontal Scroll with absolute coordinate
// INPUT:
//   HWND hwnd - window handle for setting scrolling positions
//   screen_buffer *out_buf - structure with right ViewMode and scrolls data
//   long pos - position of scroll
// OUTPUT:
//   screen_buffer *out_buf - structure with correctly modified Scroll info
void ScreenBufferSetHScroll(HWND hwnd, screen_buffer *out_buf, unsigned long pos);

// Set position of Vertical Scroll with relative coordinates
// INPUT:
//   HWND hwnd - window handle for setting scrolling positions
//   screen_buffer *out_buf - structure with right ViewMode and scrolls data
//   long delta - relative to current position of scroll
// OUTPUT:
//   screen_buffer *out_buf - structure with correctly modified Scroll info
void ScreenBufferChangeVScroll(HWND hwnd, screen_buffer *out_buf, long delta);

// Set position of Horisontal Scroll with relative coordinates
// INPUT:
//   HWND hwnd - window handle for setting scrolling positions
//   screen_buffer *out_buf - structure with right ViewMode and scrolls data
//   long delta - relative to current position of scroll
// OUTPUT:
//   screen_buffer *out_buf - structure with correctly modified Scroll info
void ScreenBufferChangeHScroll(HWND hwnd, screen_buffer *out_buf, long delta);


// Draw screen_buffer on screen with correct font, ViewMode and scroll with relative coordinates
// INPUT:
//   HWND hwnd - window handle for Drawing
//   screen_buffer *out_buf - structure
//   font_params *font - parameters of current font
void ScreenBufferDraw(HWND hwnd, screen_buffer *buffer, font_params *font);

// Clear screen buffer
// INPUT:
//   screen_buffer *buffer - pointer on screen_buffer structure
// OUTPUT:
//   screen_buffer *buffer - pointer on screen_buffer structure filled with zero values
void ScreenBufferClear(screen_buffer *buffer);

#endif // __SCREEN_BUFFER_
