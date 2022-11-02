#include "InfoScreen.h"
#include "Config.h"
#include "Button.h"

const int gitBtnID = 11;
const int menuBtnID = 12;


LRESULT CALLBACK windowHandler5(HWND, UINT, WPARAM, LPARAM);

InfoScreen::InfoScreen(HINSTANCE hInst) : Screen()
{
    initClass(hInst);
    createWindow(hInst);
}
void InfoScreen::initClass(HINSTANCE hInst)
{
    ZeroMemory(&screenClass, sizeof(WNDCLASSEX));
    screenClass.cbClsExtra = NULL;
    screenClass.cbSize = sizeof(WNDCLASSEX);
    screenClass.cbWndExtra = NULL;
    screenClass.hbrBackground = CreateSolidBrush(RGB(90, 90, 90));
    screenClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    screenClass.hIcon = NULL;
    screenClass.hIconSm = NULL;
    screenClass.hInstance = hInst;
    screenClass.lpfnWndProc = (WNDPROC)windowHandler5;
    screenClass.lpszClassName = L"Info screen";
    screenClass.lpszMenuName = NULL;
    screenClass.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&screenClass))
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            L"Main screen creation failed",
            L"Window Class Failed",
            MB_ICONERROR);
    }
}

void InfoScreen::createWindow(HINSTANCE hInst)
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

void InfoScreen::sendMessage(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (this == nullptr)
    {
        return;
    }
    windowHandler5(screenWindow, msg, wParam, lParam);
}

LRESULT CALLBACK windowHandler5(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int width;
    static int height;
    switch (msg)
    {
    case WM_SIZE: {
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        SetWindowPos(window, NULL, 0, 0, width, height, NULL);
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case menuBtnID: {
            Navigation::getNavigation()->selectScreen(MAIN_SCREEN);
            break;
        }
        case gitBtnID: {
            ShellExecute(NULL, L"open", L"https://github.com/eliseybg", NULL, NULL, SW_SHOW);
            break;
        }
        }
        break;
    }
    case WM_PAINT: {
        static HWND newGameBtn;
        if (newGameBtn != 0)
            DestroyWindow(newGameBtn);
        newGameBtn = Button::createButton(window, L"github", gitBtnID, 50, 220, 200, 60);
        static HWND menuBtn;
        if (menuBtn != 0)
            DestroyWindow(menuBtn);
        menuBtn = Button::createButton(window, L"menu", menuBtnID, 0, 0, 80, 40);
       


        HBITMAP hPrevBmp;
        PAINTSTRUCT ps;
        HDC hWndDc;
        HDC hMemDc;
        BITMAP bmp;
        hWndDc = BeginPaint(window, &ps);
        RECT rect;
        HDC wdc = GetWindowDC(window);
        SetBkMode(wdc, TRANSPARENT);
        rect.left = 50;
        rect.top = 50;
        SetTextColor(wdc, RGB(180, 180, 180));
        HFONT hfnt = CreateFont(30, 20, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Arial");
        SelectObject(wdc, hfnt);
        DrawText(wdc, L"Checkers", -1, &rect, DT_SINGLELINE | DT_NOCLIP);

        rect.left = 70;
        rect.top = 90;
        SetTextColor(wdc, RGB(180, 180, 180));
        hfnt = CreateFont(20, 10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Arial");
        SelectObject(wdc, hfnt);
        DrawText(wdc, L"BUSIR coursework, 3 year", -1, &rect, DT_SINGLELINE | DT_NOCLIP);

        rect.left = 50;
        rect.top = 140;
        SetTextColor(wdc, RGB(180, 180, 180));
        hfnt = CreateFont(30, 20, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Arial");
        SelectObject(wdc, hfnt);
        DrawText(wdc, L"Developer", -1, &rect, DT_SINGLELINE | DT_NOCLIP);

        rect.left = 70;
        rect.top = 180;
        SetTextColor(wdc, RGB(180, 180, 180));
        hfnt = CreateFont(20, 10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Arial");
        SelectObject(wdc, hfnt);
        DrawText(wdc, L"Budnitsky Elisey", -1, &rect, DT_SINGLELINE | DT_NOCLIP);

        DeleteDC(wdc);
        EndPaint(window, &ps);
        break;
    }
    case WM_DESTROY: {
        return 0;
    }
    }
    return DefWindowProc(window, msg, wParam, lParam);
}
