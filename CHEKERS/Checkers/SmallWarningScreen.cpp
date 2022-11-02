#include "SmallWarningScreen.h"
#include "Config.h"

LRESULT CALLBACK windowHandler4(HWND, UINT, WPARAM, LPARAM);

SmallWarningScreen::SmallWarningScreen(HINSTANCE hInst) : Screen()
{
    initClass(hInst);
    createWindow(hInst);
}
void SmallWarningScreen::initClass(HINSTANCE hInst)
{
    ZeroMemory(&screenClass, sizeof(WNDCLASSEX));
    screenClass.cbClsExtra = NULL;
    screenClass.cbSize = sizeof(WNDCLASSEX);
    screenClass.cbWndExtra = NULL;
    screenClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    screenClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    screenClass.hIcon = NULL;
    screenClass.hIconSm = NULL;
    screenClass.hInstance = hInst;
    screenClass.lpfnWndProc = (WNDPROC)windowHandler4;
    screenClass.lpszClassName = L"Warning screen";
    screenClass.lpszMenuName = NULL;
    screenClass.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&screenClass))
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            L"Warning screen creation failed",
            L"Window Class Failed",
            MB_ICONERROR);
    }
}

void SmallWarningScreen::createWindow(HINSTANCE hInst)
{
    screenWindow = CreateWindowEx(NULL,
        screenClass.lpszClassName,
        NULL,
        NULL,
        0,
        0,
        WIDTH,
        HEIGHT,
        NULL,
        NULL,
        hInst,
        NULL);

    LONG lStyle = GetWindowLong(screenWindow, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    SetWindowLong(screenWindow, GWL_STYLE, lStyle);

    if (!screenWindow)
    {
        int nResult = GetLastError();

        MessageBox(NULL, L"Window creation failed", L"Window Creation Failed", MB_ICONERROR);
    }
}

void SmallWarningScreen::sendMessage(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (this == nullptr)
    {
        return;
    }
    windowHandler4(screenWindow, msg, wParam, lParam);
}

LRESULT CALLBACK windowHandler4(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        SetWindowPos(window, NULL, 0, 0, width, height, NULL);

        HWND hwndButton = CreateWindow(
            L"BUTTON",
            L"OK",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10,
            10,
            100,
            100,
            window,
            reinterpret_cast<HMENU>(123),
            (HINSTANCE)GetWindowLongPtr(window, GWLP_HINSTANCE),
            NULL);
        return 0;
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case 123: {
            MessageBox(window, L"DO", L"DO", MB_ICONINFORMATION);
        }
        }
        break;
    }
    case WM_PAINT: {

        break;
    }
    case WM_DESTROY: {
        return 0;
    }
    }
    return DefWindowProc(window, msg, wParam, lParam);
}