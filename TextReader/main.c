#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

#include "ui/ui.h"
#include "globals/global.h"
#include "file_support/file_support.h"
#include "string_support/string_support.h"


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = _T("VParusovInterface");
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           _T("VParusovInterface"),/* Classname */
           _T("TextReader"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           CW_USEDEFAULT,       /* The programs width */
           CW_USEDEFAULT,       /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument         /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static global_data Globals = { 0 };
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            {
                CREATESTRUCT *OurCreateStruct = (CREATESTRUCT *)lParam;
                MyDebugMessage("WM_Create\n");
                Globals.HWnd = hwnd;
                Globals.hInstanse = OurCreateStruct->hInstance;

                // just like VS2017
                Globals.BackRed = 30;
                Globals.BackBlue = 30;
                Globals.BackGreen = 30;
                Globals.TextRed = 255;
                Globals.TextBlue = 255;
                Globals.TextGreen = 255;

                Globals.LineHeight = 32;

                // Create font I like
                Globals.hFont = CreateFont(Globals.LineHeight, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Comic Sans MS"));

                FillFontData(&Globals);

                if (MyReadFile(OurCreateStruct->lpCreateParams, &Globals))
                {
                    Globals.IsInited = TRUE;
                }
            }
            break;
        case WM_SIZE:
            MyDebugMessage("WM_Size\n");
            if (Globals.IsInited)
            {
                Globals.WindowWidth = LOWORD(lParam);
                Globals.WindowHeight = HIWORD(lParam);
                ClearDrawStrings(&Globals);
                BuildDrawStrings(&Globals);
                InvalidateRect(hwnd, NULL, TRUE);
                MyDebugMessage("Window size is %ix%i\n", Globals.WindowWidth , Globals.WindowHeight);
            }
            break;
        case WM_PAINT:
            MyDebugMessage("WM_Paint\n");
            if (Globals.IsInited)
            {
                DrawTextOnScreen(&Globals);
            }
            break;
        case WM_DESTROY:
            if (Globals.IsInited)
            {
                SafeDelete(Globals.LoadedBuffer);
                DeleteObject(Globals.hFont);
            }
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
