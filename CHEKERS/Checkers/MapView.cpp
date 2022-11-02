#include "MapView.h"
#include <cmath>

MapView::MapView()
{
	selected.x = -1;
	selected.y = -1;
	resetCheckers();
}

void MapView::resetCheckers() {
	int board[8][8] = {
			{0, 3, 0, 3, 0, 3, 0, 3},
			{3, 0, 3, 0, 3, 0, 3, 0},
			{0, 3, 0, 3, 0, 3, 0, 3},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{1, 0, 1, 0, 1, 0, 1, 0},
			{0, 1, 0, 1, 0, 1, 0, 1},
			{1, 0, 1, 0, 1, 0, 1, 0}
	};

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			tiles[i][j].checker = board[i][j];
		}
	}
	isKill = false;
	currentStep = 0;
	eraseSteps();
	currentPlayer = PLAYER_1;
}

void MapView::drawMap(HDC hWndDc, int x, int y, int _size)
{
	startX = x;
	startY = y;
	cellSize = (_size - strokeWidth) / 8;
	size = cellSize * 8 + 2 * strokeWidth;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			tiles[i][j].x = j * cellSize;
			tiles[i][j].y = i * cellSize;
		}
	}
	
	HBRUSH brushStroke = CreateSolidBrush(strokeColor);
	SelectObject(hWndDc, brushStroke);
	Rectangle(hWndDc, x, y, x + size, y + size);

	HBRUSH brushSelected = CreateSolidBrush(SelectedCellColor);
	HBRUSH brushPossibleMove = CreateSolidBrush(PossibleMoveCellColor);
	HBRUSH brushBlack = CreateSolidBrush(DarkCellColor);
	HBRUSH brushWhite = CreateSolidBrush(lightCellColor);
	int pos = 0; // if  even - white, odd - black
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pos++ % 2 == 0)
			{
				SelectObject(hWndDc, brushWhite);
			}
			else {
				SelectObject(hWndDc, brushBlack);
			}
			if (isPlayerField(selected.x, selected.y, currentPlayer) && canMove(selected.x, selected.y))
				if(selected.x == i && selected.y == j) {
					SelectObject(hWndDc, brushSelected);
				}
			if(tiles[i][j].isPossibleMove)
				SelectObject(hWndDc, brushPossibleMove);
			int posX = (int)(tiles[i][j].x + x + strokeWidth);
			int posY = (int)(tiles[i][j].y + y + strokeWidth);
			Rectangle(hWndDc, posX, posY, posX + cellSize, posY + cellSize);
		}
		pos--;
	}
	DeleteObject(brushStroke);
	DeleteObject(brushBlack);
	DeleteObject(brushWhite);
}

void MapView::getPressedCell(int _x, int _y, HWND window)
{
	int fromX = selected.x;
	int fromY = selected.y;

	if (currentStep != steps.size()) {
		if (MessageBox(window, L"You will erase your turns, continue?", L"", MB_YESNO) == IDYES)
			eraseSteps();
		else return;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tiles[i][j].checker == currentPlayer || tiles[i][j].checker == currentPlayer + 1) {
				goto endCheckWinner;
			}
		}
	}
	MessageBox(window, L"You will erase your turns, continue?", L"", MB_OK);
	resetCheckers();
endCheckWinner:

	selected.x = -1;
	selected.y = -1;
	int x = _x - startX - strokeWidth;
	int y = _y - startY - strokeWidth;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tiles[i][j].x < x && tiles[i][j].x + cellSize > x && 
				tiles[i][j].y < y && tiles[i][j].y + cellSize > y)
			{
				selected.x = i;
				selected.y = j;
				goto loop;
			}
		}
	}
loop:
	if (isKill && fromX == selected.x && fromY == selected.y) {
		changePlayer(true);
		clearPossibleMoves();
		isKill = false;
		return;
	}
	if (selected.x == -1) {
		clearPossibleMoves();
		return;
	}
	if (isKill)
		killChecker(x, y);
	else 
	canMove(fromX, fromY);
	if (tiles[selected.x][selected.y].isPossibleMove) {
		tiles[selected.x][selected.y].checker = tiles[fromX][fromY].checker;

		Step step = createStep(fromX, fromY, selected.x, selected.y);

		if (tiles[selected.x][selected.y].checker % 2 == 0
			&& isDamkaKilled(fromX, fromY, selected.x, selected.y, &step)) {
			clearPossibleMoves();
			if (checkDamka(selected.x, selected.y) == 2)
				return;
			else {
				isKill = false;
			}
		}

		if (selected.x == 0) {
			if (tiles[selected.x][selected.y].checker == 1) tiles[selected.x][selected.y].checker++;
		}
		if (selected.x == 7) {
			if (tiles[selected.x][selected.y].checker == 3) tiles[selected.x][selected.y].checker++;
		}
		tiles[fromX][fromY].checker = 0;
		if (abs(fromX - selected.x) == 2)
		{
			step.killedChecker = tiles[fromX - (fromX - selected.x) / 2][fromY - (fromY - selected.y) / 2].checker;
			step.killedX = fromX - (fromX - selected.x) / 2;
			step.killedY = fromY - (fromY - selected.y) / 2;
			tiles[fromX - (fromX - selected.x) / 2][fromY - (fromY - selected.y) / 2].checker = 0;

			if (!killChecker(selected.x, selected.y)) {
				isKill = false;
				changePlayer(false);
			}
			else {
				isKill = true;
				fromX = selected.x;
				fromY = selected.y;
			}
		}
		else {
			changePlayer(true);
		}
		addStep(step);
	}
	clearPossibleMoves();
	if (isKill) {
		selected.x = fromX;
		selected.y = fromY;
	}
}

bool MapView::isPlayerField(int x, int y, Player player) {
	return tiles[x][y].checker == player || tiles[x][y].checker == player + 1;
}

bool MapView::isDamkaKilled(int fromX, int fromY, int x, int y, Step *step) {
	if (fromX - x == 0 || fromY - y == 0) {
		changePlayer(false);
		return false;
	}
	int deltaX = (fromX - x) / abs(fromX - x);
	int deltaY = (fromY - y) / abs(fromY - y);
	bool flag = false;
	while (fromX != x) {
		if (tiles[fromX][fromY].checker != 0) {
			flag = true;
			step->killedX = fromX;
			step->killedY = fromY;
			step->killedChecker = tiles[fromX][fromY].checker;
			tiles[fromX][fromY].checker = 0;

		}
		fromX -= deltaX;
		fromY -= deltaY;
	}
	return flag;
}

Player MapView::getAnotherPlayer() {
	if (currentPlayer == PLAYER_1)
		return PLAYER_2;
	else return PLAYER_1;
}

void MapView::changePlayer(bool clear) {
	if (clear) {
		selected.x = -1;
		selected.y = -1;
	}
	currentPlayer = getAnotherPlayer();
}

bool MapView::canMove(int x, int y) {
	if (currentPlayer == PLAYER_1) {
		// check from bottom to top
		if (tiles[x][y].checker == 1)
			return killChecker(x, y) |isFreeSell(x - 1, y - 1) | isFreeSell(x - 1, y + 1) ;
	}
	else {
		// check from bottom to top
		if (tiles[x][y].checker == 3)
			return killChecker(x, y) | isFreeSell(x + 1, y - 1) | isFreeSell(x + 1, y + 1);
	}

	// check damka
	if (tiles[x][y].checker % 2 == 0)
		return checkDamka(x, y);
	return false;
}

bool MapView::checkDamka(int x, int y) {
	int _x = x;
	int _y = y;
	bool flag = 0;
	for ( ;_x >= 0 && _y >= 0; _x--, _y--)
	{
		if (x != _x && isPlayerField(_x, _y, currentPlayer))
			break;
		if (!isFreeSell(_x, _y) && !isPlayerField(_x, _y, currentPlayer))
		{
			_x--;
			_y--;
			if (_x >= 0 && _y >= 0)
				if (!isFreeSell(_x, _y))
				{
					break;
				}
				else {
					for (; _x >= 0 && _y >= 0 && isFreeSell(_x, _y); _x--, _y--){}
					flag = 2;
					break;
				}
		}
		flag = 1;
	}
	_x = x;
	_y = y;
	for (; _x >= 0 && _y < 8; _x--, _y++)
	{
		if (x != _x && isPlayerField(_x, _y, currentPlayer))
			break;
		if (!isFreeSell(_x, _y) && !isPlayerField(_x, _y, currentPlayer))
		{
			_x--;
			_y++;
			if (_x >= 0 && _y < 8)
				if (!isFreeSell(_x, _y))
				{
					break;
				}
				else {
					for (; _x >= 0 && _y >= 0 && isFreeSell(_x, _y); _x--, _y++) {}
					flag = 2;
					break;
				}
		}
		flag = true;
	}
	_x = x;
	_y = y;
	for (; _x < 8 && _y >= 0; _x++, _y--)
	{
		if (x != _x && isPlayerField(_x, _y, currentPlayer))
			break;
		if (!isFreeSell(_x, _y) && !isPlayerField(_x, _y, currentPlayer))
		{
			_x++;
			_y--;
			if (_x < 8 && _y >= 0)
				if (!isFreeSell(_x, _y))
				{
					break;
				}
				else {
					for (; _x >= 0 && _y >= 0 && isFreeSell(_x, _y); _x++, _y--) {}
					flag = 2;
					break;
				}
		}
		flag = true;
	}
	_x = x;
	_y = y;
	for (; _x < 8 && _y < 8; _x++, _y++)
	{
		if (x != _x && isPlayerField(_x, _y, currentPlayer))
			break;
		if (!isFreeSell(_x, _y) && !isPlayerField(_x, _y, currentPlayer))
		{
			_x++;
			_y++;
			if (_x < 8 && _y < 8)
				if (!isFreeSell(_x, _y))
				{
					break;
				}
				else {
					for (; _x >= 0 && _y >= 0 && isFreeSell(_x, _y); _x++, _y++) {}
					flag = 2;
					break;
				}
		}
		flag = true;
	}
	return flag;
}

bool MapView::killChecker(int x, int y) {
	return (isPlayerField(x - 1, y - 1, getAnotherPlayer()) && !isFreeSell(x - 1, y - 1) && isFreeSell(x - 2, y - 2))
		| (isPlayerField(x - 1, y + 1, getAnotherPlayer()) && !isFreeSell(x - 1, y + 1) && isFreeSell(x - 2, y + 2))
		| (isPlayerField(x + 1, y - 1, getAnotherPlayer()) && !isFreeSell(x + 1, y - 1) && isFreeSell(x + 2, y - 2))
		| (isPlayerField(x + 1, y + 1, getAnotherPlayer()) && !isFreeSell(x + 1, y + 1) && isFreeSell(x + 2, y + 2));
}

bool MapView::isFreeSell(int x, int y) {
	if (x < 0 || x > 7 || y < 0 || y > 7)
	{
		return false;
	}
	if (!tiles[x][y].checker) {
		tiles[x][y].isPossibleMove = true;
		return true;
	}
	return false;
}

void MapView::clearPossibleMoves() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) 
			tiles[i][j].isPossibleMove = false;
}

void MapView::draw(HWND hWnd, int x, int y, int _size) {
	PAINTSTRUCT ps;
	HDC hWndDc;
	hWndDc = BeginPaint(hWnd, &ps);
	drawMap(hWndDc, x, y, _size);
	drawCheckers(hWndDc, x, y);
	EndPaint(hWnd, &ps);
}

void MapView::drawCheckers(HDC hWndDc, int x, int y)
{
	HBRUSH brushBlack = CreateSolidBrush(BlackChecker);
	HBRUSH brushWhite = CreateSolidBrush(WhiteChecker);
	y = y + strokeWidth;
	x = x + strokeWidth;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (tiles[i][j].checker) {
			case 1: {
				SelectObject(hWndDc, brushWhite);
				int margin = 5;
				Ellipse(hWndDc, x + tiles[i][j].x + margin, y + tiles[i][j].y + margin, cellSize + x + tiles[i][j].x - margin, cellSize + y + tiles[i][j].y - margin);
				break; 
			}
			case 2: {
				SelectObject(hWndDc, brushWhite);
				int margin = 5;
				Ellipse(hWndDc, x + tiles[i][j].x + margin, y + tiles[i][j].y + margin, cellSize + x + tiles[i][j].x - margin, cellSize + y + tiles[i][j].y - margin);
				SelectObject(hWndDc, brushBlack);
				margin = 8;
				Ellipse(hWndDc, x + tiles[i][j].x + margin, y + tiles[i][j].y + margin, cellSize + x + tiles[i][j].x - margin, cellSize + y + tiles[i][j].y - margin);
				SelectObject(hWndDc, brushWhite);
				margin = 11;
				Ellipse(hWndDc, x + tiles[i][j].x + margin, y + tiles[i][j].y + margin, cellSize + x + tiles[i][j].x - margin, cellSize + y + tiles[i][j].y - margin);
				break;
			}
			case 3: {
				SelectObject(hWndDc, brushBlack);
				int margin = 5;
				Ellipse(hWndDc, x + tiles[i][j].x + margin, y + tiles[i][j].y + margin, cellSize + x + tiles[i][j].x - margin, cellSize + y + tiles[i][j].y - margin);
				break;
			}
			case 4: {
				SelectObject(hWndDc, brushBlack);
				int margin = 5;
				Ellipse(hWndDc, x + tiles[i][j].x + margin, y + tiles[i][j].y + margin, cellSize + x + tiles[i][j].x - margin, cellSize + y + tiles[i][j].y - margin);
				SelectObject(hWndDc, brushWhite);
				margin = 8;
				Ellipse(hWndDc, x + tiles[i][j].x + margin, y + tiles[i][j].y + margin, cellSize + x + tiles[i][j].x - margin, cellSize + y + tiles[i][j].y - margin);
				SelectObject(hWndDc, brushBlack);
				margin = 11;
				Ellipse(hWndDc, x + tiles[i][j].x + margin, y + tiles[i][j].y + margin, cellSize + x + tiles[i][j].x - margin, cellSize + y + tiles[i][j].y - margin);
				break;
			}
			}
			tiles[i][j];
		}
	}
	DeleteObject(brushBlack);
	DeleteObject(brushWhite);
}

int MapView::getSize() {
	return size;
}

Step MapView::createStep(int startX, int startY, int endX, int endY) {
	Step step;
	step.startX = startX;
	step.startY = startY;
	step.endX = endX;
	step.endY = endY;
	step.killedChecker = 2;
	step.killedX = -1;
	step.killedY = -1;
	return step;
}

void MapView::loadNext(HWND window) {
	if (currentStep + 1 > steps.size()) {
		MessageBox(window, L"That is last step", L"", MB_ICONINFORMATION);
		return;
	}
	Step step = steps[currentStep];
	tiles[step.endX][step.endY].checker = tiles[step.startX][step.startY].checker;
	tiles[step.startX][step.startY].checker = 0;
	if (step.killedX != -1) {
		tiles[step.killedX][step.killedY].checker = 0;
	}
	currentStep++;
	clearPossibleMoves();
	changePlayer(true);
}

void MapView::loadPrev(HWND window) {
	if (currentStep == 0) {
		MessageBox(window, L"That is fisrt step", L"", MB_ICONINFORMATION);
		return;
	}
	Step step = steps[--currentStep];
	tiles[step.startX][step.startY].checker = tiles[step.endX][step.endY].checker;
	tiles[step.endX][step.endY].checker = 0;
	if (step.killedX != -1) {
		tiles[step.killedX][step.killedY].checker = step.killedChecker;
	}
	clearPossibleMoves();
	changePlayer(true);
}

void MapView::addStep(Step step) {
	steps.push_back(step);
	currentStep++;
}

void MapView::eraseSteps() {
	steps.erase(steps.begin() + currentStep, steps.end());
}
