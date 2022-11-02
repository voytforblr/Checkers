#include "Game.h"
#include <algorithm>
#include <string>
#include <typeinfo>

HWND Game::MainHWND;
std::map<int, Position>* Game::ID_to_Position;
std::map<Position, Checker*>* Game::BoardPosition;
std::map<int, Checker*>* Game::BoardObj;
int Game::LengthFigure;
Checker::Type* Game::ActivePlayer;
int Game::LastIDButton;

void Game::PrintBaseInfo()
{
	
}

void Game::Start(HWND hWnd, HINSTANCE hInst)
{
	MainHWND = hWnd;
	BoardPosition = new std::map<Position, Checker*>;
	BoardObj = new std::map<int, Checker*>;
	ID_to_Position = new std::map<int, Position>;

	RECT rect;
	GetClientRect(hWnd, &rect);
	LengthFigure = rect.bottom / 8;
	bool SomeSwitch = false;
	ActivePlayer = new Checker::Type(Checker::Type::WHITE);
	Checker::Type TypeFigure = Checker::Type::BLACK;
	for (int Y = rect.top; Y < rect.bottom - 10; Y += LengthFigure) {
		if (Y == LengthFigure * 3)
			TypeFigure = Checker::Type::OPEN;
		else if (Y == LengthFigure * 5)
			TypeFigure = Checker::Type::WHITE;
		for (int X = SomeSwitch ? rect.left + LengthFigure + 10 : rect.left + 10; X < rect.bottom - 10; X += LengthFigure * 2) {
			int ID = X + Y * 10;
			(*BoardObj)[ID] = (*BoardPosition)[Position(X, Y)] = new Checker(TypeFigure, hWnd, hInst, X, Y, LengthFigure);
			(*ID_to_Position)[ID] = Position(X, Y);
		}

		SomeSwitch = !SomeSwitch;
	}
}

void Game::DrawFigures(LPDRAWITEMSTRUCT pDIS)
{
	HDC hDC = pDIS->hDC;
	RECT rc = pDIS->rcItem;
	COLORREF ColourNode;
	bool IS_WICTIM = (*BoardObj)[pDIS->CtlID]->IsWictim();
	if (IS_WICTIM)
		ColourNode = RGB(100, 100, 100);
	else
		ColourNode = RGB(0, 0, 0);
	HBRUSH hBrush = CreateSolidBrush(ColourNode);
	SelectObject(hDC, hBrush);
	Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
	DeleteObject(hBrush);

	Checker::Type ColourFigure = (*BoardObj)[pDIS->CtlID]->GetType();
	if (ColourFigure != Checker::Type::OPEN) {
		const wchar_t* Bmp = (*BoardObj)[pDIS->CtlID]->GetBMP();
		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, Bmp, IMAGE_BITMAP, rc.right, rc.bottom, LR_LOADFROMFILE);
		HDC bmpDC = CreateCompatibleDC(hDC);
		SelectObject(bmpDC, hBmp);
		TransparentBlt(hDC, rc.left, rc.top, rc.right, rc.bottom, bmpDC, rc.left, rc.top, rc.right, rc.bottom, RGB(255, 255, 255));
		DeleteObject(hBmp);
	}
}

void Game::RotateDeck()
{
	bool Switch = 1;
	for (size_t Y = 0; Y < 4; ++Y) {
		size_t X;
		for (Switch ? X = 0 : X = 1; X < 8; X += 2)
			Swap(Turn(Position(X * LengthFigure + 10, Y * LengthFigure), Position((7 - X) * LengthFigure + 10, (7 - Y) * LengthFigure)));
		Switch = !Switch;
	}
	InvalidateRect(MainHWND, 0, NULL);
}

void Game::Swap(const Turn& someTurn)
{
	int FirstID = someTurn.first.first + someTurn.first.second * 10,
		SecondID = someTurn.second.first + someTurn.second.second * 10;
	if (typeid((*BoardObj)[FirstID]).name() == typeid((*BoardObj)[SecondID]).name()) {
		Checker* checker = (*BoardObj)[FirstID];
		(*BoardPosition)[someTurn.first] = (*BoardObj)[FirstID] = (*BoardObj)[SecondID];
		(*BoardPosition)[someTurn.second] = (*BoardObj)[SecondID] = checker;
	}
	else
		if (typeid((*BoardObj)[FirstID]).name() == typeid(Checker*).name()) {
			Checker* t = (*BoardObj)[FirstID];
			Queen* checker1 = new Queen(*t);
			Checker* checker2 = new Checker(*(*BoardObj)[SecondID]);
			delete (*BoardObj)[FirstID];
			delete (*BoardObj)[SecondID];
			(*BoardObj)[FirstID] = checker1;
			(*BoardObj)[SecondID] = checker2;
		}
		else {
			Checker* checker1 = new Checker(*(*BoardObj)[FirstID]);
			Queen* checker2 = new Queen(*(*BoardObj)[SecondID]);
			delete (*BoardObj)[FirstID];
			delete (*BoardObj)[SecondID];
			(*BoardObj)[FirstID] = checker1;
			(*BoardObj)[SecondID] = checker2;
		}
}

const bool& Game::CheckForPlayer(const Position& pos)
{
	return *ActivePlayer == (*BoardPosition)[pos]->GetType();
}

void Game::SearchingForWictims()
{
	for (std::map<Position, Checker*>::const_iterator node = BoardPosition->cbegin(); node != BoardPosition->cend(); ++node) {
		if (*ActivePlayer != (*node).second->GetType())
			continue;
		(*node).second->SearchingForWictims(BoardPosition, (*node).first, *ActivePlayer);
	}
}

void Game::NoWictims()
{
	for (std::map<Position, Checker*>::const_iterator node = BoardPosition->cbegin(); node != BoardPosition->cend(); ++node)
		if ((*node).second->IsWictim())
			(*node).second->IO_Wictim();
}

const bool& Game::NeedAttack()
{
	//Algorithm
	//Algorithm
	//Algorithm
	//Algorithm
	//Algorithm
	std::map<Position, Checker*>::const_iterator node = std::find_if(BoardPosition->cbegin(), BoardPosition->cend(),
		[](std::pair<Position, Checker*> SomeObj) {
			return SomeObj.second->IsWictim();
		});
	if (node == BoardPosition->cend())
		return false;
	else
		return true;
}

const Game::State& Game::GetStateTurn(const Turn& PlayerTurn)
{
	if ((*BoardPosition)[PlayerTurn.first]->GetType() == Checker::Type::OPEN)
		return Game::State::IMPOSSIBLE;
	if ((*BoardPosition)[PlayerTurn.second]->GetType() == Checker::Type::OPEN)
		if (typeid((*BoardPosition)[PlayerTurn.first]).name() == typeid(Queen*).name()) {
			if (abs((int)PlayerTurn.first.first - (int)PlayerTurn.second.first) !=
				abs((int)PlayerTurn.first.second - (int)PlayerTurn.second.second))
				return Game::State::IMPOSSIBLE;
			int RotateX, RotateY;
			State StateTurn(Game::State::SIMPLE_TURN);
			PlayerTurn.first.first > PlayerTurn.second.first ? RotateX = -1 : RotateX = 1;
			PlayerTurn.first.second > PlayerTurn.second.second ? RotateY = -1 : RotateY = 1;
			Position WictimPos(Position(PlayerTurn.first.first + RotateX * LengthFigure, PlayerTurn.first.second + RotateY * LengthFigure));
			Position OpenPos(Position(WictimPos.first + RotateX * LengthFigure, WictimPos.second + RotateY * LengthFigure));
			while (OpenPos != PlayerTurn.second && WictimPos != PlayerTurn.second) {
				if ((*BoardPosition)[WictimPos]->GetType() != Checker::Type::OPEN &&
					(*BoardPosition)[WictimPos]->GetType() != *ActivePlayer &&
					(*BoardPosition)[OpenPos]->GetType() == Checker::Type::OPEN)
					if (StateTurn == Game::State::ATTACK)
						return Game::State::IMPOSSIBLE;
					else
						StateTurn = Game::State::ATTACK;
				WictimPos.first += RotateX * LengthFigure; WictimPos.second += RotateY * LengthFigure;
				OpenPos.first += RotateX * LengthFigure; OpenPos.second += RotateY * LengthFigure;
			}
			if ((*BoardPosition)[WictimPos]->GetType() != Checker::Type::OPEN &&
				(*BoardPosition)[WictimPos]->GetType() != *ActivePlayer &&
				StateTurn != Game::State::ATTACK)
				StateTurn = Game::State::ATTACK;
			return StateTurn;
		}
		else {
			Position pos(GetWictimPos(PlayerTurn));
			if (abs((int)PlayerTurn.first.first - (int)PlayerTurn.second.first) == 2 * LengthFigure &&
				abs((int)PlayerTurn.first.second - (int)PlayerTurn.second.second) == 2 * LengthFigure &&
				(*BoardPosition)[pos]->IsWictim())
				return Game::State::ATTACK;
			else if (abs((int)PlayerTurn.first.first - (int)PlayerTurn.second.first) == LengthFigure &&
				((int)PlayerTurn.first.second - (int)PlayerTurn.second.second) == LengthFigure)
				return Game::State::SIMPLE_TURN;
		}
	return Game::State::IMPOSSIBLE;

}

const Position& Game::GetWictimPos(const Turn& turn)
{
	return (*BoardPosition)[turn.first]->GetWictimPos(turn, BoardPosition);
}

void Game::MakeTurn(const int& FirstIDNode, const int& SecondIDNode)
{

	Turn turn = Turn((*ID_to_Position)[FirstIDNode], (*ID_to_Position)[SecondIDNode]);
	Game::State TurnState = GetStateTurn(turn);

	if ((NeedAttack() && TurnState != Game::State::ATTACK) || TurnState == Game::State::IMPOSSIBLE) {
		return;
	}
	if (TurnState == Game::State::ATTACK) {
		Position WictimPos(GetWictimPos(turn));
		ToKill(WictimPos);
		NoWictims();
		Swap(turn);
		SearchingForWictims();
		CheckIsQueen(turn.second);
		InvalidateRect(MainHWND, 0, 0);
		if (NeedAttack() && LastIDButton != FirstIDNode) {
			LastIDButton = SecondIDNode;
			return;
		}
	}
	if (TurnState != Game::State::ATTACK)
		Swap(turn);
	if (GetWinner() == Checker::Type::OPEN) {
		CheckIsQueen(turn.second);
		RotateDeck();
		*ActivePlayer == Checker::Type::BLACK ? *ActivePlayer = Checker::Type::WHITE :
			*ActivePlayer = Checker::Type::BLACK;
		NoWictims();
		SearchingForWictims();
	}
}

const Checker::Type& Game::GetWinner()
{
	//Algorithm
	//Algorithm
	//Algorithm
	//Algorithm
	//Algorithm
	int WhiteScore = std::count_if(BoardPosition->cbegin(), BoardPosition->cend(),
		[](std::pair<Position, Checker*> SomeObj) {
			return SomeObj.second->GetType() == Checker::Type::BLACK;
		});
	int BlackScore = std::count_if(BoardPosition->cbegin(), BoardPosition->cend(),
		[](std::pair<Position, Checker*> SomeObj) {
			return SomeObj.second->GetType() == Checker::Type::WHITE;
		});
	if (12 - WhiteScore == 12) return Checker::Type::WHITE;
	if (12 - BlackScore == 12) return Checker::Type::BLACK;
	return Checker::Type::OPEN;
}

void Game::CheckIsQueen(const Position& NodePos)
{
	if (!(*BoardPosition)[NodePos]->IsQueen())
		if (NodePos.second == 0) {
			Queen* NewQueen = new Queen(*(*BoardPosition)[NodePos]);
			delete (*BoardPosition)[NodePos];
			int id = NodePos.second * 10 + NodePos.first;
			(*BoardObj)[id] = (*BoardPosition)[NodePos] = NewQueen;
		}
}

void Game::ClearMemory()
{
	delete ActivePlayer;
	for (std::map<int, Checker*>::const_iterator It = BoardObj->cbegin(); It != BoardObj->cend(); ++It)
		delete (*It).second;
	delete BoardObj;
	delete BoardPosition;
	delete ID_to_Position;
}

const HWND& Game::GetButton(const int& ButtonID) { return (*BoardObj)[ButtonID]->GetButton(); }
const Checker::Type& Game::GetActivePlayer() { return *ActivePlayer; }
const Checker::Type& Game::GetColourFigure(const int& IDFigure) { return (*BoardObj)[IDFigure]->GetType(); }