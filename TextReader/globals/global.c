#include "global.h"

#include <stdio.h>
#include <stdarg.h>

void MyDebugMessage(const char *format, ...)
{
#ifdef MY_DEBUG
    va_list arglist;
    va_start (arglist, format);
    vprintf( format, arglist );
    va_end( arglist );
#endif
}
