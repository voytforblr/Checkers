#pragma once
#include "framework.h"
#include <map>

using Position = std::pair<int, int>;
using Turn = std::pair<Position, Position>;

class Checker
{
public:
	enum class Type { WHITE, BLACK, OPEN };
	Checker(const Type& SomeType, HWND hWnd, HINSTANCE hInst, int X, int Y, int Length);
	Checker(const Checker& Obj);
	virtual const bool& IsQueen();
	const bool& IsWictim();
	const Checker::Type& GetType() const;
	void SetType(const Checker::Type& NewType);
	void IO_Wictim();
	const HWND& GetButton();
	const wchar_t* GetBMP();
	virtual void SearchingForWictims(std::map<Position, Checker*>* BoardPosition, const Position& Pos, const Type& ActivePlayer);
	const bool& CheckIsWictim(const Position& WictimPos, const Position& NewPos,
		std::map<Position, Checker*>* BoardPosition, const Type& ActivePlayer);
	virtual const Position& GetWictimPos(const Turn& turn, std::map<Position, Checker*>* BoardPosition);

	~Checker();
protected:
	Checker();
	int X, Y;
	int Length;
	bool* isWictim;
	const wchar_t* BMP;
	Type* TypeCheck;
	HWND* button;
};
