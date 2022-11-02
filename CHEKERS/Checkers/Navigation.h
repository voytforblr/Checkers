#pragma once
#include "Screen.h"
#include <Windows.h>
enum NavScreen 
{
	SLASH_SCREEN,
	MAIN_SCREEN,
	GAME_SCREEN,
	INFO_SCREEN,
	SMALL_SCREEN_WARNING
};
const int amountOfScreens = 4;

class Container 
{
public:
	Screen* screen = nullptr;
};

class Navigation 
{
public:
	Navigation(Navigation& other) = delete;
	void operator=(const Navigation&) = delete;
	static void init(HINSTANCE, HWND);
	static Navigation* getNavigation();
	void notify(HWND, UINT, WPARAM, LPARAM);
	void notifyAll(HWND, UINT, WPARAM, LPARAM);
	void selectScreen(NavScreen);
	void selectScreen(Screen*);
	void isScreenWarning(bool, HWND);
protected:
	Navigation(HINSTANCE, HWND);
	static Navigation* navigation;
	Screen* currentScreen;
	Container screens[amountOfScreens];
};

