#include "MainScreen.h"
#include "Config.h"
#include "Button.h"

const int newGameBtnID = 0;
const int lastGameBtnID = 1;
const int infoBtnID = 2;
const int exitBtnID = 3;

LRESULT CALLBACK windowHandler2(HWND, UINT, WPARAM, LPARAM);

MainScreen::MainScreen(HINSTANCE hInst): Screen()
{
    initClass(hInst);
    createWindow(hInst);
}
void MainScreen::initClass(HINSTANCE hInst)
{
    ZeroMemory(&screenClass, sizeof(WNDCLASSEX));
    screenClass.cbClsExtra = NULL;
    screenClass.cbSize = sizeof(WNDCLASSEX);
    screenClass.cbWndExtra = NULL;
    screenClass.hbrBackground = CreateSolidBrush(RGB(120, 0, 0));
    screenClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    screenClass.hIcon = NULL;
    screenClass.hIconSm = NULL;
    screenClass.hInstance = hInst;
    screenClass.lpfnWndProc = (WNDPROC) windowHandler2;
    screenClass.lpszClassName = L"Main screen";
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

void MainScreen::createWindow(HINSTANCE hInst)
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

void MainScreen::sendMessage(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (this == nullptr)
    {
        return;
    }
    windowHandler2(screenWindow, msg, wParam, lParam);
}

HWND createButton(HWND window, const wchar_t text[], int id, int x, int y, int width, int height) {
    return CreateWindow(
        L"BUTTON",
        text,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x,
        y,
        width,
        height,
        window,
        reinterpret_cast<HMENU>(id),
        (HINSTANCE)GetWindowLongPtr(window, GWLP_HINSTANCE),
        NULL);
}

LRESULT CALLBACK windowHandler2(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
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
                case newGameBtnID: {
                    Navigation::getNavigation()->selectScreen(GAME_SCREEN);
                    break;
                }
                case infoBtnID: {
                    Navigation::getNavigation()->selectScreen(INFO_SCREEN);
                    break;
                }
                case exitBtnID: {
                    PostQuitMessage(0);
                    break;
                }
            }
            break;
        }
        case WM_PAINT: {
            int btnWidth = 250;
            int btnHeight = 40;
            int btnMargin = 30;
            int btnN = 4;
            int btnX = (width - btnWidth) / 2;
            int btnY = (height - btnN * btnHeight - (btnN - 1) * btnMargin) / 2;
            int deltaY = btnHeight + btnMargin;
            static HWND newGameBtn;
            if (newGameBtn != 0)
                DestroyWindow(newGameBtn);
            newGameBtn = Button::createButton(window, L"NEW GAME", newGameBtnID, btnX, btnY, btnWidth, btnHeight);
            btnY += deltaY;
            static HWND infoBtn;
            if (infoBtn != 0)
                DestroyWindow(infoBtn);
            infoBtn = Button::createButton(window, L"INFO", infoBtnID, btnX, btnY, btnWidth, btnHeight);
            btnY += deltaY;
            static HWND exitBtn;
            if (exitBtn != 0)
                DestroyWindow(exitBtn);
            exitBtn = Button::createButton(window, L"EXIT", exitBtnID, btnX, btnY, btnWidth, btnHeight);

            break;
        }
        case WM_DESTROY: {
            return 0;
        }
    }
    return DefWindowProc(window, msg, wParam, lParam);
}
