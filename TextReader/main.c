#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

#include "globals/global.h"

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
    wincl.style = CS_DBLCLKS | CS_CLASSDC | CS_OWNDC; /* Catch double-clicks and request own DC*/
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
           WS_OVERLAPPEDWINDOW | WS_VSCROLL, /* default window */
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
                Globals.hInstanse = OurCreateStruct->hInstance;

                // just like VS2017

                CustomisationSetDefault(hwnd, &Globals.Customization);
                InputBufferInit(&Globals.LoadedBuffer);
                ScreenBufferInit(&Globals.DrawBuffer);

                if (InputBufferReadBuffer(OurCreateStruct->lpCreateParams, &Globals.LoadedBuffer))
                {
                    Globals.IsInited = TRUE;
                }
            }
            break;
        case WM_SIZE:
            MyDebugMessage("WM_Size\n");
            if (!Globals.IsInited)
            {
                break;
            }
            {
                long v_scroll_pos = Globals.DrawBuffer.ScrollH;
                Globals.WindowWidth = LOWORD(lParam);
                Globals.WindowHeight = HIWORD(lParam);


                ScreenBufferClear(&Globals.DrawBuffer);
                ScreenBufferBuild(&Globals.LoadedBuffer, &Globals.DrawBuffer, &Globals.Customization.Font, Globals.WindowWidth, Globals.WindowHeight);

                Globals.DrawBuffer.ScrollH = v_scroll_pos;
                // set scroll params
                if (Globals.DrawBuffer.WindowHeightInLines > Globals.DrawBuffer.Size)
                {
                    ShowScrollBar(hwnd, SB_VERT, FALSE);
                }
                else
                {
                    ShowScrollBar(hwnd, SB_VERT, TRUE);
                    SetScrollRange(hwnd, SB_VERT, 0, Globals.DrawBuffer.Size - Globals.DrawBuffer.WindowHeightInLines, FALSE);
                    if (Globals.DrawBuffer.ScrollH > Globals.DrawBuffer.Size - Globals.DrawBuffer.WindowHeightInLines)
                        Globals.DrawBuffer.ScrollH = Globals.DrawBuffer.Size - Globals.DrawBuffer.WindowHeightInLines;
                    SetScrollPos(hwnd, SB_VERT, Globals.DrawBuffer.ScrollH, TRUE);
                }
                InvalidateRect(hwnd, NULL, TRUE);
                MyDebugMessage("Window size is %ix%i\n", Globals.WindowWidth , Globals.WindowHeight);
            }
            break;
        case WM_PAINT:
            MyDebugMessage("WM_Paint\n");
            if (Globals.IsInited)
            {
                ScreenBufferDraw(hwnd, &Globals.DrawBuffer, &Globals.Customization.Font);
            }
            break;
        case WM_VSCROLL:
            if (!Globals.IsInited)
                break;
            else
            {
                int action = LOWORD(wParam);
                int pos = 0;

                if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK)
                {
                    pos = HIWORD(wParam);
                }
                else if (action == SB_LINEDOWN)
                {
                    pos = Globals.DrawBuffer.ScrollH + 1;
                }
                else if (action == SB_LINEUP)
                {
                    if (Globals.DrawBuffer.ScrollH > 0)
                        pos = Globals.DrawBuffer.ScrollH - 1;
                    else
                        pos = 0;
                }
                else
                    break;

                if (pos == -1)
                    break;

                Globals.DrawBuffer.ScrollH = pos;
                if (Globals.DrawBuffer.ScrollH > Globals.DrawBuffer.Size - Globals.DrawBuffer.WindowHeightInLines)
                    Globals.DrawBuffer.ScrollH = Globals.DrawBuffer.Size - Globals.DrawBuffer.WindowHeightInLines;
                InvalidateRect(hwnd, NULL, TRUE);
                SetScrollPos(hwnd, SB_VERT, Globals.DrawBuffer.ScrollH, TRUE);
            }
            break;
        case WM_DESTROY:
            if (Globals.IsInited)
            {
                InputBufferClear(&Globals.LoadedBuffer);
                ScreenBufferClear(&Globals.DrawBuffer);
                CustomisationClear(&Globals.Customization);
            }
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
