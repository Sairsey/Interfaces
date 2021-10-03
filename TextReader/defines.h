#ifndef __DEFINES_H_
#define __DEFINES_H_

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>

#include "windows.h"

// flag for enabling logging
//#define MY_DEBUG

#define TRUE 1
#define FALSE 0

#define TRU TRUE
#define FOLS FALSE

typedef int bool;

void MyDebugMessage(const char *format, ...);

#define SafeDelete(x) if ((x) != NULL) free((x));

#define LITTLE_VSCROLL 1
#define BIG_VSCROLL 10

#define LITTLE_HSCROLL 1
#define BIG_HSCROLL 10

#define HOLD_DELAY 0.1 * CLOCKS_PER_SEC

#endif // __DEFINES_H_
