#include "screen_buffer.h"
#include "math.h"

void ScreenBufferInit(screen_buffer *buffer)
{
    buffer->Data = NULL;
    buffer->Lengths = NULL;
    buffer->Size = 0;
    buffer->ScrollH = 0;
    buffer->ViewMode = FORMATTED;
    buffer->WindowHeightInLines = 0;
}

void ScreenBufferBuild(input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height)
{
    int CurY = 0;              // current line in out_buf
    int screen_buffer_len = 0; // maximum length of line on screen
    int input_buffer_line = 0; // index of line in in_buf
    int i;

    if (out_buf->ViewMode == FORMATTED)
        screen_buffer_len = window_width / font->MinSymbolWidth;
    else
        screen_buffer_len = in_buf->MaxLineLength;

    if (screen_buffer_len == 0)
    {
        screen_buffer_len = 1;
    }

    // lets count out_buf->Size
    out_buf->Size = 0;
    for (i = 0; i < in_buf->NumberOfLines; i++)
        out_buf->Size += CustomisationGetTextLineScreenLines(font, in_buf->Lines[i], window_width);

    out_buf->Data = malloc(sizeof(char *) * out_buf->Size);
    if (!out_buf->Data)
    {
        MyDebugMessage("Cannot allocate buffer...\n");
        return;
    }

    out_buf->Lengths = malloc(sizeof(unsigned long) * out_buf->Size);
    if (!out_buf->Lengths)
    {
        free(out_buf->Data);
        out_buf->Data = NULL;
        MyDebugMessage("Cannot allocate buffer...\n");
        return;
    }

    // main logic here
    // for every line in in_buf
    CurY = 0;
    for (input_buffer_line = 0; input_buffer_line < in_buf->NumberOfLines; input_buffer_line++)
    {
        char *line_element = in_buf->Lines[input_buffer_line];
        char *screen_line_start_element = in_buf->Lines[input_buffer_line];
        unsigned long screen_line_element_index = 0;
        unsigned long screen_line_width = 0;

        // check if it is empty line
        if (*line_element == 0)
        {
            out_buf->Data[CurY] = line_element;
            out_buf->Lengths[CurY++] = 0;
            continue;
        }

        // for every element in our line
        for (;*line_element != 0; line_element++)
        {
            if (screen_line_element_index < screen_buffer_len && // if we not out of screen_buffer_len
                (out_buf->ViewMode == UNFORMATTED  // if unformatted
                || (out_buf->ViewMode == FORMATTED && screen_line_width + font->SymbolWidth[(unsigned char )*line_element] < window_width))) // or formatted but have enough space
            {
                screen_line_element_index++;
                screen_line_width += font->SymbolWidth[(unsigned char )*line_element];
            }
            else // copy tmp_buffer to out_buf and memset new one
            {
                out_buf->Data[CurY] = screen_line_start_element;
                out_buf->Lengths[CurY++] = screen_line_element_index;
                screen_line_start_element = line_element;
                screen_line_element_index = 0;
                screen_line_width = 0;
                line_element--;      // still load symbol
            }
        }

        if (screen_line_element_index != 0)
        {
            out_buf->Data[CurY] = screen_line_start_element;
            out_buf->Lengths[CurY++] = screen_line_element_index;
        }
    }

    out_buf->WindowHeightInLines = window_height / font->BaselineToBaseline;
    if (out_buf->WindowHeightInLines == 0)
        out_buf->WindowHeightInLines = 1;

}

void ScreenBufferClear(screen_buffer *buffer)
{
    if (buffer->Data)
    {
        free(buffer->Data);
        buffer->Data = NULL;
    }
    if (buffer->Lengths)
    {
        free(buffer->Lengths);
        buffer->Lengths = NULL;
    }
    buffer->ScrollH = 0;
    buffer->ViewMode = FORMATTED;
    buffer->Size = 0;
    buffer->WindowHeightInLines = 0;
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

    for (int i = 0; i < buffer->WindowHeightInLines; i++)
    {
        int len = buffer->Lengths[i + buffer->ScrollH];
        TextOut(hDC, ScreenRect.left, ScreenRect.top + i * font->BaselineToBaseline, buffer->Data[i + buffer->ScrollH], len);
    }

    EndPaint(hwnd, &PaintStruct);
}
