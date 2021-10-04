#include "win_callbacks.h"
#include "../menu_defines.h"

// Callback on message WM_CREATE
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnCreate(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd)
{
    CREATESTRUCT *OurCreateStruct = (CREATESTRUCT *)lParam;
    MyDebugMessage("WM_Create\n");
    Globals->hInstanse = OurCreateStruct->hInstance;

    // just like VS2017
    CustomizationSetDefault(hWnd, &Globals->Customization);
    InputBufferInit(&Globals->LoadedBuffer);
    ScreenBufferInit(&Globals->DrawBuffer);

    if (InputBufferReadBuffer(OurCreateStruct->lpCreateParams, &Globals->LoadedBuffer))
    {
        Globals->IsInited = TRUE;
    }
}

// Callback on message WM_SIZE
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnSize(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd)
{
    MyDebugMessage("WM_Size\n");
    if (!Globals->IsInited)
        return;

    Globals->WindowWidth = LOWORD(lParam);
    Globals->WindowHeight = HIWORD(lParam);

    ScreenBufferResize(hWnd, &Globals->LoadedBuffer, &Globals->DrawBuffer, &Globals->Customization.Font, Globals->WindowWidth, Globals->WindowHeight);
    MyDebugMessage("Window size is %ix%i\n", Globals->WindowWidth , Globals->WindowHeight);
}

// Callback on message WM_PAINT
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnPaint(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd)
{
    MyDebugMessage("WM_Paint\n");
    if (Globals->IsInited)
        ScreenBufferDraw(hWnd, &Globals->DrawBuffer, &Globals->Customization.Font);
}

// Callback on message WM_VSCROLL
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnVScroll(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd)
{
    if (!Globals->IsInited)
        return;

    switch (LOWORD(wParam))
    {
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            ScreenBufferSetVScroll(hWnd, &Globals->DrawBuffer, HIWORD(wParam));
            break;
        case SB_LINEDOWN:
            ScreenBufferChangeVScroll(hWnd, &Globals->DrawBuffer, LITTLE_VSCROLL);
            break;
        case SB_LINEUP:
            ScreenBufferChangeVScroll(hWnd, &Globals->DrawBuffer, -LITTLE_VSCROLL);
            break;
        default:
            break;
    }

}

// Callback on message WM_HSCROLL
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnHScroll(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd)
{
    if (!Globals->IsInited)
        return;

    switch (LOWORD(wParam))
    {
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            ScreenBufferSetHScroll(hWnd, &Globals->DrawBuffer, HIWORD(wParam));
            break;
        case SB_LINEDOWN:
            ScreenBufferChangeHScroll(hWnd, &Globals->DrawBuffer, LITTLE_HSCROLL);
            break;
        case SB_LINEUP:
            ScreenBufferChangeHScroll(hWnd, &Globals->DrawBuffer, LITTLE_HSCROLL);
            break;
        default:
            break;
    }
}

// Callback on message WM_MOUSEWHEEL
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnMouseWheel(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd)
{
    if (!Globals->IsInited)
        return;

    long zDelta = -GET_WHEEL_DELTA_WPARAM(wParam) / 120;
    ScreenBufferChangeVScroll(hWnd, &Globals->DrawBuffer, LITTLE_VSCROLL * zDelta);
}

// Callback on message WM_KEYDOWN
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnKeyDown(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd)
{
    if (!Globals->IsInited)
        return;

    static DWORD prev_key = -1;
    static clock_t prev_time = 0;
    clock_t cur_time = clock();

    if (prev_key == wParam && cur_time - prev_time <= HOLD_DELAY)
        return;

    prev_key = wParam;
    prev_time = cur_time;

    switch (wParam)
    {
        case VK_UP:
            ScreenBufferChangeVScroll(hWnd, &Globals->DrawBuffer, -LITTLE_VSCROLL);
            break;
        case VK_DOWN:
            ScreenBufferChangeVScroll(hWnd, &Globals->DrawBuffer, LITTLE_VSCROLL);
            break;
        case VK_LEFT:
            ScreenBufferChangeHScroll(hWnd, &Globals->DrawBuffer, -LITTLE_HSCROLL);
            break;
        case VK_RIGHT:
            ScreenBufferChangeHScroll(hWnd, &Globals->DrawBuffer, LITTLE_HSCROLL);
            break;
        case VK_PRIOR:
            ScreenBufferChangeVScroll(hWnd, &Globals->DrawBuffer, -BIG_VSCROLL);
            break;
        case VK_NEXT:
            ScreenBufferChangeVScroll(hWnd, &Globals->DrawBuffer, BIG_VSCROLL);
            break;
        default:
            break;
    }
}

// Callback on message WM_COMMAND
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnCommand(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd)
{
    if (!Globals->IsInited)
        return;

    switch (LOWORD(wParam))
    {
        case IDM_ABOUT:
            MessageBox(hWnd, "Made by Vladimir Parusov\n"
                    "5030102/90201 group\n"
                    "Year 2021", "About", MB_OK);
            break;
        case IDM_VIEW_WRAP:
            {
                DWORD state ;
                state = GetMenuState(GetMenu(hWnd), IDM_VIEW_WRAP, MF_BYCOMMAND);
                if (state & MF_CHECKED)
                {
                    CheckMenuItem(GetMenu(hWnd), IDM_VIEW_WRAP, MF_BYCOMMAND | MF_UNCHECKED);
                    Globals->DrawBuffer.ViewMode = UNFORMATTED;
                }
                else
                {
                    CheckMenuItem(GetMenu(hWnd), IDM_VIEW_WRAP, MF_BYCOMMAND | MF_CHECKED);
                    Globals->DrawBuffer.ViewMode = FORMATTED;
                }
                ScreenBufferResize(hWnd, &Globals->LoadedBuffer, &Globals->DrawBuffer, &Globals->Customization.Font, Globals->WindowWidth, Globals->WindowHeight);
            }
            break;
        default:
            break;
    }

}

// Callback on message WM_DESTROY
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnDestroy(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd)
{
    if (Globals->IsInited)
    {
        InputBufferClear(&Globals->LoadedBuffer);
        ScreenBufferClear(&Globals->DrawBuffer);
        CustomizationClear(&Globals->Customization);
    }
}
