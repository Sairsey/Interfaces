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
    unsigned long ScrollH;  // Scroll over Y coord
    mode ViewMode;          // mode of view
    unsigned long WindowHeightInLines; // Height of window in lines;
} screen_buffer;

void ScreenBufferInit(screen_buffer *buffer);

void ScreenBufferBuild(input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height);

void ScreenBufferDraw(HWND hwnd, screen_buffer *buffer, font_params *font);

void ScreenBufferClear(screen_buffer *buffer);

#endif // __SCREEN_BUFFER_
