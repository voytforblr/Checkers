#include "SplashScreen.h"
#include "Config.h"
#include "resource.h"
#include "splash_styles.h"

LRESULT CALLBACK windowHandler1(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInst;
int timer;
SplashScreen::SplashScreen(HINSTANCE _hInst) : Screen()
{
    hInst = _hInst;
    initClass(hInst);
    createWindow(hInst);
}

void SplashScreen::initClass(HINSTANCE hInst)
{
    ZeroMemory(&screenClass, sizeof(WNDCLASSEX));
    screenClass.cbClsExtra = NULL;
    screenClass.cbSize = sizeof(WNDCLASSEX);
    screenClass.cbWndExtra = NULL;
    screenClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    screenClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    screenClass.hIcon = NULL;
    screenClass.hIconSm = NULL;
    screenClass.hInstance = hInst;
    screenClass.lpfnWndProc = (WNDPROC) windowHandler1;
    screenClass.lpszClassName = L"Splash screen";
    screenClass.lpszMenuName = NULL;
    screenClass.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&screenClass))
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            L"Splash screen creation failed",
            L"Window Class Failed",
            MB_ICONERROR);
    }
}

void SplashScreen::createWindow(HINSTANCE hInst)
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

void SplashScreen::sendMessage(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (this == nullptr)
    {
        return;
    }
    windowHandler1(screenWindow, msg, wParam, lParam);
}

VOID drawImg(HWND hWnd, HBITMAP hBitmap)
{
    HBITMAP hPrevBmp;
    PAINTSTRUCT ps;
    HDC hWndDc;
    HDC hMemDc;
    BITMAP bmp;
    RECT wndRect;

    GetClientRect(hWnd, &wndRect);
    hWndDc = BeginPaint(hWnd, &ps);

    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    hMemDc = CreateCompatibleDC(hWndDc);
    hPrevBmp = (HBITMAP)SelectObject(hMemDc, hBitmap);
    BitBlt(hWndDc, 0, 0, bmp.bmWidth, bmp.bmHeight,
        hMemDc, 0, 0, SRCAND);
    SelectObject(hMemDc, hBitmap);

    DeleteDC(hMemDc);
    DeleteObject(hPrevBmp);
    EndPaint(hWnd, &ps);
}

HBITMAP hBitmap;
LRESULT CALLBACK windowHandler1(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int width;
    static int height;
    switch (msg)
    {
    case WM_TIMER:
        KillTimer(window, timer);
        Navigation::getNavigation()->selectScreen(MAIN_SCREEN);
        break;
    case WM_CREATE: {
        SetTimer(window,
            timer,
            30,
            (TIMERPROC)NULL);
        hBitmap = (HBITMAP)LoadImage(hInst, L"logo_break_time.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        break;
    }
    case WM_SIZE: {
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        SetWindowPos(window, NULL, 0, 0, width, height, NULL);
        break;
    }
    case WM_PAINT: {
        HBITMAP hPrevBmp;
        PAINTSTRUCT ps;
        HDC hWndDc;
        HDC hMemDc;
        BITMAP bmp;
        hWndDc = BeginPaint(window, &ps);
        GetObject(hBitmap, sizeof(BITMAP), &bmp);
        hMemDc = CreateCompatibleDC(hWndDc);
        hPrevBmp = (HBITMAP)SelectObject(hMemDc, hBitmap);

        int textWidth = 11 * fontWidth;
        int imgX = (width - bmp.bmWidth - textWidth) / 2;
        int imgY = (height - bmp.bmHeight) / 2;

        int textX = imgX + bmp.bmWidth;
        int textY = (height - fontHeight) / 2;

        BitBlt(hWndDc, imgX, imgY, bmp.bmWidth, bmp.bmHeight,
            hMemDc, 0, 0, SRCAND);
        SelectObject(hMemDc, hBitmap);

        RECT rect;
        HDC wdc = GetWindowDC(window);
        SetBkMode(wdc, TRANSPARENT);
        rect.left = textX;
        rect.top = textY;
        SetTextColor(wdc, RGB(15, 34, 102));
        SelectObject(wdc, hfnt);
        DrawText(wdc, L"Break time", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
        DeleteDC(wdc);
        DeleteDC(hMemDc);
        DeleteObject(hPrevBmp);
        EndPaint(window, &ps);
        break;
    }
    case WM_DESTROY: {
        return 0;
    }
    }
    return DefWindowProc(window, msg, wParam, lParam);
}
