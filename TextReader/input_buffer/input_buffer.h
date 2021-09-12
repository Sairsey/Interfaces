#ifndef __INPUT_BUFFER_
#define __INPUT_BUFFER_
#include "../defines.h"

typedef struct
{
    char *Data; //characters
    long Size;  // number of characters
} input_buffer;

void InputBufferInit(input_buffer *buffer);

bool InputBufferReadBuffer(const char *filename, input_buffer *buffer);

void InputBufferClear(input_buffer *buffer);

#endif // __INPUT_BUFFER_
