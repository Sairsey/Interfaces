#include "screen_buffer.h"

void ScreenBufferInit(screen_buffer *buffer)
{
    buffer->Data = NULL;
    buffer->Size = 0;
}

void ScreenBufferBuild(input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height)
{
    char *tmp_buffer = NULL;
    int CurY = 0;
    int LoadedBufferIndex = 0;
    int tmp_buffer_len = window_width / font->MinSymbolWidth;
    if (tmp_buffer_len == 0)
    {
        tmp_buffer_len = 1;
    }

    out_buf->Size = window_height / font->BaselineToBaseline;
    if (out_buf->Size == 0)
    {
        out_buf->Size = 1;
    }

    out_buf->Data = malloc(sizeof(char *) * out_buf->Size);
    if (!out_buf->Data)
    {
        MyDebugMessage("Cannot allocate buffer...\n");
        return;
    }

    tmp_buffer = malloc(sizeof(char) * (tmp_buffer_len + 1));
    if (!tmp_buffer)
    {
        MyDebugMessage("Cannot allocate buffer...\n");
        free(out_buf->Data);
        out_buf->Data = NULL;
        out_buf->Size = 0;
        return;
    }

    // main logic here
    for (CurY = 0; CurY < out_buf->Size; CurY++)
    {
        int CurWidth = 0, CurX = 0; // cur line width in pixels
        // zeros
        memset(tmp_buffer, 0, tmp_buffer_len + 1);
        while (CurX < tmp_buffer_len &&   // not out of tmp_buffer
               LoadedBufferIndex < in_buf->Size && // not out of LoadedBuffer
               in_buf->Data[LoadedBufferIndex] != '\n' && // not new line
               CurWidth + font->SymbolWidth[(unsigned char )in_buf->Data[LoadedBufferIndex]] < window_width // not out of screen
               )
        {
            tmp_buffer[CurX] = in_buf->Data[LoadedBufferIndex];
            CurWidth += font->SymbolWidth[(unsigned char )in_buf->Data[LoadedBufferIndex]];
            CurX++;
            LoadedBufferIndex++;
        }

        if (in_buf->Data[LoadedBufferIndex] == '\n')
            LoadedBufferIndex++;

        out_buf->Data[CurY] = malloc(sizeof(char) * (CurX + 1));
        if (!out_buf->Data[CurY])
        {
            MyDebugMessage("Apchih-ba\n");
            out_buf->Data = NULL;
            return;
        }
        memcpy(out_buf->Data[CurY], tmp_buffer, CurX + 1);
    }
}

void ScreenBufferClear(screen_buffer *buffer)
{
    if (buffer->Data)
    {
        int i = 0;
        for (i = 0; i < buffer->Size; i++)
            free(buffer->Data[i]);
        free(buffer->Data);
        buffer->Data = NULL;
    }
    buffer->Size = 0;
}

void ScreenBufferDraw(HWND hwnd, screen_buffer *buffer, font_params *font)
{
    HDC hDC;
    PAINTSTRUCT PaintStruct;
    hDC = BeginPaint(hwnd, &PaintStruct);
    RECT ScreenRect;  // Client rect

    if (buffer == NULL || buffer->Data == NULL)
    {
        MyDebugMessage("Somebody once... Run ScreenBufferDraw with bad buffer \n");
        return;
    }

    // Get client area size
    GetClientRect(hwnd, &ScreenRect);

    for (int i = 0; i < buffer->Size; i++)
    {
        int len = strlen(buffer->Data[i]);
        TextOut(hDC, ScreenRect.left, ScreenRect.top + i * font->BaselineToBaseline, buffer->Data[i], len);
    }

    EndPaint(hwnd, &PaintStruct);
}
