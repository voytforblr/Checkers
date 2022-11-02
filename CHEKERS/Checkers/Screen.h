#pragma once
#include <Windows.h>

class Screen {
public:
	virtual void sendMessage(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual HWND getWindow() = 0;
	WNDCLASSEX screenClass;
	HWND screenWindow;
};