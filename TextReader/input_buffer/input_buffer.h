#ifndef __INPUT_BUFFER_
#define __INPUT_BUFFER_
#include "../defines.h"

typedef struct
{
    char *Data; //characters
    unsigned long Size;  // number of characters
    char **Lines;         // pointers on every file Line
    unsigned long NumberOfLines; // Number of lines
    unsigned long MaxLineLength;  // Maximum line length
} input_buffer;

void InputBufferInit(input_buffer *buffer);

bool InputBufferReadBuffer(const char *filename, input_buffer *buffer);

void InputBufferClear(input_buffer *buffer);

#endif // __INPUT_BUFFER_
