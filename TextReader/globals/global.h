/* File with common used types/functions/etc. */
#ifndef __GLOBALS_H_
#define __GLOBALS_H_

// flag for enabling logging
#define MY_DEBUG

#define TRUE 1
#define FALSE 0

#define TRU TRUE
#define FOLS FALSE

typedef int bool;

typedef struct tag_global_data
{
    bool IsInited;         // is system inited
    char *LoadedBuffer;    // pointer on loaded data
    long LoadedBufferSize; // size of loaded buffer
} global_data;

void MyDebugMessage(const char *format, ...);

#endif // __GLOBALS_H_
