#pragma once
#include <Windows.h>
#include "Screen.h"
#include "Navigation.h"
class MainScreen: public Screen
{
public:
	MainScreen(HINSTANCE);
	void sendMessage(HWND, UINT, WPARAM, LPARAM);
	HWND getWindow() {
		return screenWindow;
	}
private:
	HWND screenWindow;
	void initClass(HINSTANCE);
	void createWindow(HINSTANCE);
};

