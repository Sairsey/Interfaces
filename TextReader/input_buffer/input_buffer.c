#include <stdio.h>
#include "input_buffer.h"

void InputBufferInit(input_buffer *buffer)
{
    buffer->Data = NULL;
    buffer->Size = 0;
}

bool InputBufferReadBuffer(const char *filename, input_buffer *buffer)
{
    FILE *F = NULL;
    if ((F = fopen(filename, "rt")) == NULL)
    {
        MyDebugMessage("Cannot open file %s\n", filename);
        return FALSE;
    }
    fseek(F, 0, SEEK_END);
    buffer->Size = ftell(F);
    fseek(F, 0, SEEK_SET);

    buffer->Data = (char *)malloc((buffer->Size + 1) * sizeof(char));
    if (!buffer->Data)
    {
        MyDebugMessage("Cannot allocate memory\n");
        return FALSE;
    }
    buffer->Data[buffer->Size] = 0;

    fread(buffer->Data, sizeof(char), buffer->Size, F);
    fclose(F);
    return TRUE;
}

void InputBufferClear(input_buffer *buffer)
{
    if (buffer->Data != NULL)
    {
        free(buffer->Data);
        buffer->Data = NULL;
    }
    buffer->Size = 0;
}
