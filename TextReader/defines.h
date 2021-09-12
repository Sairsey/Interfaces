#ifndef __DEFINES_H_
#define __DEFINES_H_
#include "windows.h"

// flag for enabling logging
#define MY_DEBUG

#define TRUE 1
#define FALSE 0

#define TRU TRUE
#define FOLS FALSE

typedef int bool;

void MyDebugMessage(const char *format, ...);

#define SafeDelete(x) if ((x) != NULL) free((x));

#endif // __DEFINES_H_
