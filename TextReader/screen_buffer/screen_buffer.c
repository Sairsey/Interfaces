#include "screen_buffer.h"
#include "math.h"

// Initialize screen buffer
// INPUT:
//   screen_buffer *buffer - pointer on screen_buffer structure
// OUTPUT:
//   screen_buffer *buffer - pointer on screen_buffer structure filled with zero values
void ScreenBufferInit(screen_buffer *buffer)
{
    buffer->Data = NULL;
    buffer->Size = 0;
    buffer->ScrollH = 0;
    buffer->ScrollV = 0;
    buffer->ViewMode = FORMATTED;
    buffer->WindowHeightInLines = 0;
    buffer->MaxLineLength = 0;
    buffer->WindowWidthInMinSymbols = 0;
}

// Build screen buffer
// INPUT:
//   input_buffer *in_buf - structure with data and metadata from file.
//   screen_buffer *out_buf - structure with right ViewMode
//   font_params *font - structure with metadata about current font (width of characters, height and so on)
//   long window_width, - size of window client area
//   long window_height - size of window client area
// OUTPUT:
//   screen_buffer *out_buf - structure filled with correct data and ready do be used in ScreenBufferDraw
void ScreenBufferBuild(input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height)
{
    unsigned long CurY = 0;              // current line in out_buf
    unsigned long screen_buffer_len = 0; // maximum length of line on screen
    unsigned long input_buffer_line = 0; // index of line in in_buf
    unsigned long i;

    screen_buffer_len = window_width / font->MinSymbolWidth;

    if (screen_buffer_len == 0)
    {
        screen_buffer_len = 1;
    }

    out_buf->WindowWidthInMinSymbols = screen_buffer_len;

    if (out_buf->ViewMode == UNFORMATTED)
        screen_buffer_len = in_buf->MaxLineLength;

    out_buf->MaxLineLength = screen_buffer_len;

    // lets count out_buf->Size
    if (out_buf->ViewMode == FORMATTED)
    {
        out_buf->Size = 0;
        for (i = 0; i < in_buf->NumberOfLines; i++)
            out_buf->Size += CustomizationGetTextLineScreenLines(font, in_buf->Lines[i], window_width);
    }
    else
    {
        out_buf->Size = in_buf->NumberOfLines;
    }

    out_buf->Data = malloc(sizeof(char *) * out_buf->Size);
    if (!out_buf->Data)
    {
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
            out_buf->Data[CurY++] = line_element;
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
                out_buf->Data[CurY++] = screen_line_start_element;
                screen_line_start_element = line_element;
                screen_line_element_index = 0;
                screen_line_width = 0;
                line_element--;      // still load symbol
            }
        }

        if (screen_line_element_index != 0)
        {
            out_buf->Data[CurY++] = screen_line_start_element;
        }
    }

    out_buf->WindowHeightInLines = window_height / font->BaselineToBaseline;
    if (out_buf->WindowHeightInLines == 0)
        out_buf->WindowHeightInLines = 1;

}

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
void ScreenBufferResize(HWND hwnd, input_buffer *in_buf, screen_buffer *out_buf, font_params *font, long window_width, long window_height)
{
    char *upper_left_symbol = NULL;
    mode format_state = out_buf->ViewMode;
    unsigned long v_scroll_pos = out_buf->ScrollV;
    unsigned long h_scroll_pos = out_buf->ScrollH;

    if (out_buf->Data != NULL)
        upper_left_symbol = out_buf->Data[out_buf->ScrollV] + out_buf->ScrollH;

    ScreenBufferClear(out_buf);

    out_buf->ViewMode = format_state;
    ScreenBufferBuild(in_buf, out_buf, font, window_width, window_height);

    // Putting scroll in right place.
    if (upper_left_symbol != NULL)
    {
        unsigned long r = out_buf->Size;
        unsigned long l = 0;
        // binary search for finding right position in text. log(75000) isn`t that big
        while (TRU)
        {
            v_scroll_pos = (r - l) / 2 + l;
            if (upper_left_symbol - out_buf->Data[v_scroll_pos] >
               out_buf->Data[v_scroll_pos + 1] - out_buf->Data[v_scroll_pos] - 1)
               l = v_scroll_pos + 1;
            else if (out_buf->Data[v_scroll_pos] - upper_left_symbol > 0)
                r = v_scroll_pos - 1;
            else
                break;
        }

        // if we need to place h_scroll_pos correctly
        if (out_buf->ViewMode == UNFORMATTED)
            h_scroll_pos =  upper_left_symbol - out_buf->Data[v_scroll_pos];
    }

    // set scroll params
    if (out_buf->WindowHeightInLines > out_buf->Size)
    {
        ShowScrollBar(hwnd, SB_VERT, FALSE);
    }
    else
    {
        ShowScrollBar(hwnd, SB_VERT, TRUE);
        SetScrollRange(hwnd, SB_VERT, 0, out_buf->Size - out_buf->WindowHeightInLines, FALSE);
        ScreenBufferSetVScroll(hwnd, out_buf, v_scroll_pos);
    }

    if (out_buf->ViewMode != UNFORMATTED)
    {
        ShowScrollBar(hwnd, SB_HORZ, FALSE);
    }
    else
    {
        ShowScrollBar(hwnd, SB_HORZ, TRUE);
        SetScrollRange(hwnd, SB_HORZ, 0, out_buf->MaxLineLength, FALSE);
        ScreenBufferSetHScroll(hwnd, out_buf, h_scroll_pos);
    }
    InvalidateRect(hwnd, NULL, TRUE);
}

// Set position of Vertical Scroll with absolute coordinate
// INPUT:
//   HWND hwnd - window handle for setting scrolling positions
//   screen_buffer *out_buf - structure with right ViewMode and scrolls data
//   long pos - position of scroll
// OUTPUT:
//   screen_buffer *out_buf - structure with correctly modified Scroll info
void ScreenBufferSetVScroll(HWND hwnd, screen_buffer *out_buf, unsigned long pos)
{
    if (pos == -1)
        return;

    MyDebugMessage("VScroll %i to %i", out_buf->ScrollV, pos);
    out_buf->ScrollV = pos;
    if (out_buf->ScrollV > out_buf->Size - out_buf->WindowHeightInLines)
        out_buf->ScrollV = out_buf->Size - out_buf->WindowHeightInLines;
    InvalidateRect(hwnd, NULL, TRUE);
    SetScrollPos(hwnd, SB_VERT, out_buf->ScrollV, TRUE);
}

// Set position of Horisontal Scroll with absolute coordinate
// INPUT:
//   HWND hwnd - window handle for setting scrolling positions
//   screen_buffer *out_buf - structure with right ViewMode and scrolls data
//   long pos - position of scroll
// OUTPUT:
//   screen_buffer *out_buf - structure with correctly modified Scroll info
void ScreenBufferSetHScroll(HWND hwnd, screen_buffer *out_buf, unsigned long pos)
{
    if (pos == -1 || out_buf->ViewMode != UNFORMATTED)
        return;

    out_buf->ScrollH = pos;
    if (out_buf->ScrollH > out_buf->MaxLineLength - out_buf->WindowWidthInMinSymbols)
        out_buf->ScrollH = out_buf->MaxLineLength - out_buf->WindowWidthInMinSymbols;
    InvalidateRect(hwnd, NULL, TRUE);
    SetScrollPos(hwnd, SB_HORZ, out_buf->ScrollH, TRUE);
}

// Set position of Vertical Scroll with relative coordinates
// INPUT:
//   HWND hwnd - window handle for setting scrolling positions
//   screen_buffer *out_buf - structure with right ViewMode and scrolls data
//   long delta - relative to current position of scroll
// OUTPUT:
//   screen_buffer *out_buf - structure with correctly modified Scroll info
void ScreenBufferChangeVScroll(HWND hwnd, screen_buffer *out_buf, long delta)
{
    // compare as double to avoid all tricks with comparison of signed/unsigned
    if ((double)out_buf->ScrollV + (double)delta < 0)
        delta = -out_buf->ScrollV;
    ScreenBufferSetVScroll(hwnd, out_buf, out_buf->ScrollV + delta);
}

// Set position of Horisontal Scroll with relative coordinates
// INPUT:
//   HWND hwnd - window handle for setting scrolling positions
//   screen_buffer *out_buf - structure with right ViewMode and scrolls data
//   long delta - relative to current position of scroll
// OUTPUT:
//   screen_buffer *out_buf - structure with correctly modified Scroll info
void ScreenBufferChangeHScroll(HWND hwnd, screen_buffer *out_buf, long delta)
{
    // compare as double to avoid all tricks with comparison of signed/unsigned
    if ((double)out_buf->ScrollH + (double)delta < 0)
        delta = -out_buf->ScrollH;
    ScreenBufferSetHScroll(hwnd, out_buf, out_buf->ScrollH + delta);
}

// Clear screen buffer
// INPUT:
//   screen_buffer *buffer - pointer on screen_buffer structure
// OUTPUT:
//   screen_buffer *buffer - pointer on screen_buffer structure filled with zero values
void ScreenBufferClear(screen_buffer *buffer)
{
    if (buffer->Data)
    {
        free(buffer->Data);
        buffer->Data = NULL;
    }
    buffer->ScrollH = 0;
    buffer->ScrollV = 0;
    buffer->ViewMode = FORMATTED;
    buffer->Size = 0;
    buffer->WindowHeightInLines = 0;
    buffer->MaxLineLength = 0;
    buffer->WindowWidthInMinSymbols = 0;
}

// Draw screen_buffer on screen with correct font, ViewMode and scroll with relative coordinates
// INPUT:
//   HWND hwnd - window handle for Drawing
//   screen_buffer *out_buf - structure
//   font_params *font - parameters of current font
void ScreenBufferDraw(HWND hwnd, screen_buffer *buffer, font_params *font)
{
    HDC hDC;
    PAINTSTRUCT PaintStruct;
    RECT ScreenRect;  // Client rect

    hDC = BeginPaint(hwnd, &PaintStruct);

    if (buffer == NULL || buffer->Data == NULL)
    {
        MyDebugMessage("Somebody once... Run ScreenBufferDraw with bad buffer \n");
        return;
    }

    // Get client area size
    GetClientRect(hwnd, &ScreenRect);

    for (int i = 0; i < buffer->WindowHeightInLines; i++)
    {
        unsigned long line_index = i + buffer->ScrollV;
        int len = buffer->MaxLineLength;

        if (line_index != buffer->Size)
            len = buffer->Data[line_index + 1] - buffer->Data[line_index];

        len -= buffer->ScrollH;
        TextOut(hDC, ScreenRect.left, ScreenRect.top + i * font->BaselineToBaseline, &buffer->Data[line_index][buffer->ScrollH], len);
    }

    EndPaint(hwnd, &PaintStruct);
}
