#include <stdio.h>
#include "input_buffer.h"

void InputBufferInit(input_buffer *buffer)
{
    buffer->Data = NULL;
    buffer->Size = 0;
    buffer->NumberOfLines = 0;
    buffer->Lines = NULL;
    buffer->MaxLineLength = 0;
}

bool InputBufferReadBuffer(const char *filename, input_buffer *buffer)
{
    FILE *F = NULL;
    char *tmp = NULL;
    unsigned long CurLine = 0;
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

    // update size
    buffer->Size = fread(buffer->Data, sizeof(char), buffer->Size, F);
    buffer->Data[buffer->Size] = 0;

    fclose(F);

    // split data on lines
    buffer->NumberOfLines = 1;

    for (tmp = buffer->Data; *tmp != 0; tmp++)
        if (*tmp == '\n')
            buffer->NumberOfLines++;

    buffer->Lines = (char **)malloc(buffer->NumberOfLines * sizeof(char *));
    if (!buffer->Lines)
    {
        free(buffer->Data);
        buffer->Size = 0;
        buffer->NumberOfLines = 0;
        MyDebugMessage("Cannot allocate memory\n");
        return FALSE;
    }

    buffer->Lines[CurLine++] = buffer->Data;
    for (tmp = buffer->Data; *tmp != 0; tmp++)
        if (*tmp == '\n')
        {
            *tmp = 0;
            buffer->Lines[CurLine++] = tmp + 1;
        }

    for (CurLine = 0; CurLine < buffer->NumberOfLines; CurLine++)
    {
        int line_len = strlen(buffer->Lines[CurLine]);
        buffer->MaxLineLength = line_len > buffer->MaxLineLength ? line_len : buffer->MaxLineLength;
    }

    return TRUE;
}

void InputBufferClear(input_buffer *buffer)
{
    if (buffer->Lines != NULL)
    {
        free(buffer->Lines);
        buffer->Lines = NULL;
    }
    buffer->NumberOfLines = 0;
    buffer->MaxLineLength = 0;

    if (buffer->Data != NULL)
    {
        free(buffer->Data);
        buffer->Data = NULL;
    }
    buffer->Size = 0;
}
