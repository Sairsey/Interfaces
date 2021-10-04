/* File with common used types/functions/etc. */
#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include <tchar.h>
#include <windows.h>

// include all important data
#include "../defines.h"
#include "../input_buffer/input_buffer.h"
#include "../screen_buffer/screen_buffer.h"
#include "../customization/customization.h"


// Struct that represents all data we have in this project
typedef struct tag_global_data
{
    bool IsInited;         // TRUE if system is inited. FALSE otherwise

    input_buffer LoadedBuffer; // model

    screen_buffer DrawBuffer; // view

    long WindowWidth;      // Width of window
    long WindowHeight;     // Height of window
    HINSTANCE hInstanse;   // Handle Instance

    customization_params Customization; // parameters of customizations
} global_data;



#endif // __GLOBALS_H_
