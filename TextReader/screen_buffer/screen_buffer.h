#ifndef __SCREEN_BUFFER_
#define __SCREEN_BUFFER_
#include "../defines.h"
#include "../customization/customization.h"
#include "../input_buffer/input_buffer.h"


typedef struct
{
    char **Data;  // lines
    int Size;     // number of lines
} screen_buffer;

void ScreenBufferInit(screen_buffer *buffer);

void ScreenBufferBuild(input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height);

void ScreenBufferDraw(HWND hwnd, screen_buffer *buffer, font_params *font);

void ScreenBufferClear(screen_buffer *buffer);

#endif // __SCREEN_BUFFER_
