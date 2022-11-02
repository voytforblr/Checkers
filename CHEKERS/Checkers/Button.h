#pragma once
#include <functional>
#include <Windows.h>

static class Button {
public:
    static HWND createButton(HWND window, const wchar_t text[], int id, int x, int y, int width, int height) {
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
};