#pragma once
#include <Windows.h>
#include "Screen.h"
#include "Navigation.h"

class InfoScreen : public Screen
{
public:
	InfoScreen(HINSTANCE);
	void sendMessage(HWND, UINT, WPARAM, LPARAM);
	HWND getWindow() {
		return screenWindow;
	}
private:
	HWND screenWindow;
	void initClass(HINSTANCE);
	void createWindow(HINSTANCE);
};
