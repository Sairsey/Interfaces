#include "file_support.h"
#include <stdio.h>
#include <stdlib.h>

bool MyReadFile(const char *filename, global_data *globals)
{
    FILE *F = NULL;
    if ((F = fopen(filename, "rt")) == NULL)
    {
        MyDebugMessage("Cannot open file %s\n", filename);
        return FALSE;
    }
    fseek(F, 0, SEEK_END);
    globals->LoadedBufferSize = ftell(F);
    fseek(F, 0, SEEK_SET);

    globals->LoadedBuffer = (char *)malloc((globals->LoadedBufferSize + 1) * sizeof(char));
    if (!globals->LoadedBuffer)
    {
        MyDebugMessage("Cannot allocate memory\n");
        return FALSE;
    }
    globals->LoadedBuffer[globals->LoadedBufferSize] = 0;

    fread(globals->LoadedBuffer, sizeof(char), globals->LoadedBufferSize, F);
    fclose(F);
    return TRUE;
}
