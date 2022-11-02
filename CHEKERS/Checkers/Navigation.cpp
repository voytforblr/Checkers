#include "Navigation.h"
#include "SplashScreen.h"
#include "MainScreen.h"
#include "GameScreen.h"
#include "InfoScreen.h"
#include "SmallWarningScreen.h"

Navigation* Navigation::navigation = nullptr;

Navigation::Navigation(HINSTANCE hInst, HWND holder)
{
	Screen* screen = new SplashScreen(hInst);
	SetParent(screen->getWindow(), holder);
	screens[SLASH_SCREEN].screen = screen;
	currentScreen = screen;
	ShowWindow(screen->getWindow(), SW_SHOW);
	screen = new MainScreen(hInst);
	SetParent(screen->getWindow(), holder);
	screens[MAIN_SCREEN].screen = screen;
	screen = new GameScreen(hInst);
	SetParent(screen->getWindow(), holder);
	screens[GAME_SCREEN].screen = screen;
	screen = new InfoScreen(hInst);
	SetParent(screen->getWindow(), holder);
	screens[INFO_SCREEN].screen = screen;
	screen = new SmallWarningScreen(hInst);
	SetParent(screen->getWindow(), holder);
	screens[SMALL_SCREEN_WARNING].screen = screen;
}

void Navigation::notify(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	currentScreen->sendMessage(handleforwindow, msg, wParam, lParam);
}

void Navigation::notifyAll(HWND handleforwindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < amountOfScreens; i++) {
		screens[i].screen->sendMessage(handleforwindow, msg, wParam, lParam);
	}
}

void Navigation::selectScreen(NavScreen screenType)
{
	ShowWindow(currentScreen->getWindow(), SW_HIDE);
	currentScreen = screens[screenType].screen;
	ShowWindow(currentScreen->getWindow(), SW_SHOW);
}

void Navigation::selectScreen(Screen* screen)
{
	ShowWindow(currentScreen->getWindow(), SW_HIDE);
	currentScreen = screen;
	ShowWindow(currentScreen->getWindow(), SW_SHOW);
}

bool wasScreenSmall = false;
void Navigation::isScreenWarning(bool isSmall, HWND window)
{
	if ((!wasScreenSmall && !isSmall) || (wasScreenSmall && isSmall)) {
		return;
	}

	static Screen* temp = currentScreen;
	if (isSmall && !wasScreenSmall) {
		selectScreen(SMALL_SCREEN_WARNING);
		wasScreenSmall = true;
	}
	else {
		selectScreen(temp);
		wasScreenSmall = false;
	}
}

Navigation* Navigation::getNavigation()
{
	return navigation;
}

void Navigation::init(HINSTANCE hInst, HWND holder)
{
	if (navigation == nullptr) {
		navigation = new Navigation(hInst, holder);
	}
}
