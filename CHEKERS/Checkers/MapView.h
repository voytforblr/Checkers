#pragma once
#include <Windows.h>
#include "Config.h"
#include <vector>

enum Player {
    PLAYER_1 = 1, PLAYER_2 = 3
};
typedef struct {
    int x;
    int y;
    int checker;
    int isPossibleMove;
} Cell;

typedef struct {
    int x;
    int y;
} Coord;

typedef struct {
    int board[8][8];
    int startX;
    int startY;
    int endX;
    int endY;
    int killedChecker;
    int killedX;
    int killedY;
    Player player;
    bool isKill;
} Step;

class MapView
{
public:
    MapView();
    void draw(HWND, int, int, int);
    int getSize();
    void resetCheckers();
    void getPressedCell(int, int, HWND);
    bool canMove(int, int);
    bool isPlayerField(int, int, Player);
    void loadNext(HWND);
    void loadPrev(HWND);
private:
    void drawMap(HDC, int, int, int);
    void drawCheckers(HDC, int, int);
    bool isFreeSell(int, int);
    bool killChecker(int, int);
    Player getAnotherPlayer();
    void changePlayer(bool);
    bool checkDamka(int, int);
    bool isDamkaKilled(int, int, int, int, Step*);
    float strokeWidth = 10;
    void clearPossibleMoves();
    COLORREF lightCellColor = RGB(236, 218, 185);
    COLORREF SelectedCellColor = RGB(128, 0, 0);
    COLORREF PossibleMoveCellColor = RGB(128, 128, 0);
    COLORREF DarkCellColor = RGB(175, 137, 104);
    COLORREF strokeColor = RGB(105, 40, 37);
    COLORREF textColor = RGB(255, 255, 255);
    COLORREF BlackChecker = RGB(0, 0, 0);
    COLORREF WhiteChecker = RGB(255, 255, 255);
    Step createStep(int, int, int, int);
    Cell tiles[8][8];
    Coord selected;
    int size;
    int cellSize;
    int startX;
    int startY;
    bool isKill = false;
    Player currentPlayer = PLAYER_1;
    std::vector<Step> steps;
    int currentStep = 0;
    void addStep(Step);
    void eraseSteps();
};



