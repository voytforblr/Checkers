#include <windows.h>
#include "Navigation.h"
#include "Config.h"
#include <iostream>

WNDCLASSEX createClassHolder(HINSTANCE);
HWND createWindowHolder(HINSTANCE, WNDCLASSEX);
LRESULT CALLBACK windowHolderHandler(HWND, UINT, WPARAM, LPARAM);
HINSTANCE _hInst;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASSEX windowsHolder = createClassHolder(hInst);
    HWND windowHolder = createWindowHolder(hInst, windowsHolder);
    ShowWindow(windowHolder, nShowCmd);

    _hInst = hInst;

    HACCEL hAccelTable = LoadAccelerators(hInst, L"checkers");
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

WNDCLASSEX createClassHolder(HINSTANCE hInst) {
    WNDCLASSEX holderClass;
    ZeroMemory(&holderClass, sizeof(WNDCLASSEX));
    holderClass.cbClsExtra = NULL;
    holderClass.cbSize = sizeof(WNDCLASSEX);
    holderClass.cbWndExtra = NULL;
    holderClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    holderClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    holderClass.hIcon = NULL;
    holderClass.hIconSm = NULL;
    holderClass.hInstance = hInst;
    holderClass.lpfnWndProc = (WNDPROC)windowHolderHandler;
    holderClass.lpszClassName = L"window holder";
    holderClass.lpszMenuName = NULL;
    holderClass.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&holderClass))
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            L"Class holder creation failed",
            L"Window Class Failed",
            MB_ICONERROR);
    }
    return holderClass;
}
HWND createWindowHolder(HINSTANCE hInst, WNDCLASSEX classHolder) {
    HWND window = CreateWindowEx(NULL,
        classHolder.lpszClassName,
        L"Checkers",
        WS_OVERLAPPEDWINDOW,
        200,
        150,
        WIDTH,
        HEIGHT,
        NULL,
        NULL,
        hInst,
        NULL
    );
    if (!window)
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            L"Window creation failed",
            L"Window Creation Failed",
            MB_ICONERROR);
    }
    return window;
}
LRESULT CALLBACK windowHolderHandler(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
            lpMMI->ptMinTrackSize.x = 400;
            lpMMI->ptMinTrackSize.y = 300;
        }
        case WM_CREATE: {
            Navigation::init(_hInst, window);
        }
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);

           // Navigation::getNavigation()->isScreenWarning(width < WIDTH || height < HEIGHT, window);
            Navigation::getNavigation()->notifyAll(window, msg, wParam, lParam);
            break;
        }
        case WM_COMMAND: {
            Navigation::getNavigation()->notify(window, msg, wParam, lParam);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
    }
    return DefWindowProc(window, msg, wParam, lParam);
}