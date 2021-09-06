#include "string_support.h"


void BuildDrawStrings(global_data *globals)
{
    char *tmp_buffer = NULL;
    int CurY = 0;
    int LoadedBufferIndex = 0;
    int tmp_buffer_len = globals->WindowWidth / globals->MinSymbolWidth;
    if (tmp_buffer_len == 0)
    {
        tmp_buffer_len = 1;
    }

    globals->DrawBufferLines = globals->WindowHeight / globals->BaselineToBaseline;
    if (globals->DrawBufferLines == 0)
    {
        globals->DrawBufferLines = 1;
    }

    globals->DrawBuffer = malloc(sizeof(char *) * globals->DrawBufferLines);
    if (!globals->DrawBuffer)
    {
        MyDebugMessage("Cannot allocate buffer...\n");
        return;
    }

    tmp_buffer = malloc(sizeof(char) * (tmp_buffer_len + 1));
    if (!tmp_buffer)
    {
        MyDebugMessage("Cannot allocate buffer...\n");
        free(globals->DrawBuffer);
        globals->DrawBuffer = NULL;
        return;
    }

    // main logic here
    for (CurY = 0; CurY < globals->DrawBufferLines; CurY++)
    {
        int CurWidth = 0, CurX = 0; // cur line width in pixels
        // zeros
        memset(tmp_buffer, 0, tmp_buffer_len + 1);
        while (CurX < tmp_buffer_len &&                      // not out of tmp_buffer
               LoadedBufferIndex < globals->LoadedBufferSize && // not out of LoadedBuffer
               globals->LoadedBuffer[LoadedBufferIndex] != '\n' && // not new line
               CurWidth + globals->SymbolWidth[(unsigned char )globals->LoadedBuffer[LoadedBufferIndex]] < globals->WindowWidth // not out of screen
               )
        {
            tmp_buffer[CurX] = globals->LoadedBuffer[LoadedBufferIndex];
            CurWidth += globals->SymbolWidth[(unsigned char )globals->LoadedBuffer[LoadedBufferIndex]];
            CurX++;
            LoadedBufferIndex++;
        }

        if (globals->LoadedBuffer[LoadedBufferIndex] == '\n')
            LoadedBufferIndex++;

        globals->DrawBuffer[CurY] = malloc(sizeof(char) * (CurX + 1));
        if (!globals->DrawBuffer[CurY])
        {
            MyDebugMessage("Apchih-ba\n");
            globals->DrawBuffer = NULL;
            return;
        }
        memcpy(globals->DrawBuffer[CurY], tmp_buffer, CurX + 1);
    }



}

void ClearDrawStrings(global_data *globals)
{
    if (globals->DrawBuffer)
    {
        int i = 0;
        for (i = 0; i < globals->DrawBufferLines; i++)
            free(globals->DrawBuffer[i]);
        free(globals->DrawBuffer);
        globals->DrawBuffer = NULL;
    }
}
