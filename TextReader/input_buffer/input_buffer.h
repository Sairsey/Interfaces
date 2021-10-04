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

// Initialize input buffer
// INPUT:
//   input_buffer *buffer - pointer on input_buffer structure
// OUTPUT:
//   input_buffer *buffer - pointer on input_buffer structure filled with zero values
void InputBufferInit(input_buffer *buffer);

// Read data from file and fill all necessary data metadata in input buffer
// INPUT:
//   const char *filename - path to file
// OUTPUT:
//   bool (returned) - TRUE if operation ended successfully, FALSE otherwise
//   input_buffer *buffer - pointer on input_buffer structure filled with data and metadata if operation ended successfully.
//                          otherwise filled with zeroes
bool InputBufferReadBuffer(const char *filename, input_buffer *buffer);

// Clear input buffer
// INPUT:
//   input_buffer *buffer - pointer on input_buffer structure
// OUTPUT:
//   input_buffer *buffer - pointer on input_buffer structure filled with zero values
void InputBufferClear(input_buffer *buffer);

#endif // __INPUT_BUFFER_
