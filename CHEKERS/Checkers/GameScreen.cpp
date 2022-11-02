#include "GameScreen.h"
#include "Config.h"
#include "MapView.h"
#include "Button.h"
#include "game_styles.h"
#include "Game.h"



LRESULT CALLBACK windowHandler3(HWND, UINT, WPARAM, LPARAM);
MapView* map = new MapView();
const int menuBtnID = 11;
const int backStepBtnID = 12;
const int nextStepBtnID = 13;
const int menuBtnRestertID = 14;

char szMenu[] = "IDC_CHECKERS";

GameScreen::GameScreen(HINSTANCE _hInst) : Screen()
{
    initClass(_hInst);
    createWindow(_hInst);

   

}

void GameScreen::initClass(HINSTANCE hInst)
{
    ZeroMemory(&screenClass, sizeof(WNDCLASSEX));
    screenClass.cbClsExtra = NULL;
    screenClass.cbSize = sizeof(WNDCLASSEX);
    screenClass.cbWndExtra = NULL;
    screenClass.hbrBackground = CreateSolidBrush(RGB(80, 90, 0));
    screenClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    screenClass.hIcon = NULL;
    screenClass.hIconSm = NULL;
    screenClass.hInstance = hInst;
    screenClass.lpfnWndProc = (WNDPROC) windowHandler3;
    screenClass.lpszClassName = L"Game screen";
    screenClass.lpszMenuName =L"IDC_CHECKERS";// MAKEINTRESOURCEW(IDC_CHECKERS);
    screenClass.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&screenClass))
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            L"Game screen creation failed",
            L"Window Class Failed",
            MB_ICONERROR);
    }


}

void GameScreen::createWindow(HINSTANCE hInst)
{
    screenWindow = CreateWindowEx(NULL,
        screenClass.lpszClassName,
        NULL,
        WS_OVERLAPPEDWINDOW,
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

void GameScreen::sendMessage(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (this == nullptr)
    {
        return;
    }
    windowHandler3(screenWindow, msg, wParam, lParam);
}

LRESULT CALLBACK windowHandler3(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{   

    

    static int width;
    static int height;
    switch (msg)
    {
    case WM_CREATE: {
       
    }
    case WM_SIZE: {
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        SetWindowPos(window, NULL, 0, 0, width, height, NULL);
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case menuBtnID: {
            Navigation::getNavigation()->selectScreen(MAIN_SCREEN);
            map->resetCheckers();
            break;
        }
        case menuBtnRestertID: {
            if (MessageBox(window, L"Start new game?", L"", MB_YESNO) == IDYES) {
                map->resetCheckers();
                InvalidateRect(window, 0, 0);
            }
            break;
        }
        case backStepBtnID: {
            map->loadPrev(window);
            InvalidateRect(window, 0, 0);
            break;
        }
        case nextStepBtnID: {
            map->loadNext(window);
            InvalidateRect(window, 0, 0);
            break;
        }
        }
        break;
    }
    case WM_LBUTTONDOWN: {
        map->getPressedCell(LOWORD(lParam), HIWORD(lParam), window);
        InvalidateRect(window, 0, 0);
        break;
    }
    case WM_PAINT: {
        int mapSize;
        if (height * 0.8 < width * 0.6)
        {
            mapSize = height * 0.8;
        }
        else mapSize = width * 0.6;
        int mapX = (width - mapSize) / 2;
        int mapY = (height - mapSize) / 2;

        map->draw(window, mapX, mapY, mapSize);

        int size = map->getSize();

        int btnY = height - mapY + 5;

        static HWND menuBtn;
        if (menuBtn != 0)
            DestroyWindow(menuBtn);
        menuBtn = Button::createButton(window, L"menu", menuBtnID, 0, 0, 80, 40);
        static HWND resetBtn;
        if (resetBtn != 0)
            DestroyWindow(resetBtn);
        resetBtn = Button::createButton(window, L"reset", menuBtnRestertID, mapX, 0, size, mapY);
        static HWND backStepBtn;
        if (backStepBtn != 0)
            DestroyWindow(backStepBtn);
        backStepBtn = Button::createButton(window, L"<=", backStepBtnID, mapX, btnY, size / 2, mapY - 5);
        static HWND nextStepBtn;
        if (nextStepBtn != 0)
            DestroyWindow(nextStepBtn);
        nextStepBtn = Button::createButton(window, L"=>", nextStepBtnID, mapX + size / 2, btnY, size / 2, mapY - 5);
        break;
    }
    case WM_DESTROY: {
        return 0;
    }
    }
    return DefWindowProc(window, msg, wParam, lParam);
}
