#include "../globals/global.h"

// File with callbacks just for more comfortable reading of WndProc in main.c


// Callback on message WM_CREATE
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnCreate(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd);

// Callback on message WM_SIZE
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnSize(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd);

// Callback on message WM_PAINT
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnPaint(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd);

// Callback on message WM_VSCROLL
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnVScroll(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd);

// Callback on message WM_HSCROLL
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnHScroll(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd);

// Callback on message WM_MOUSEWHEEL
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnMouseWheel(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd);

// Callback on message WM_KEYDOWN
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnKeyDown(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd);

// Callback on message WM_COMMAND
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnCommand(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd);

// Callback on message WM_DESTROY
// INPUT:
//   global_data *Globals - all application data, that stored as static variable field in WndProc
//   WPARAM wParam - data that was sent to WndProc
//   LPARAM lParam - data that was sent to WndProc
//   HWND hWnd - data that was sent to WndProc
void OnDestroy(global_data *Globals, WPARAM wParam, LPARAM lParam, HWND hWnd);
