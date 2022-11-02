#include "Checker.h"

Checker::Checker(const Type& SomeType, HWND hWnd, HINSTANCE hInst, int X, int Y, int Length) : isWictim(new bool),/* isQueen(new bool),*/ TypeCheck(new Type),
button(new HWND)
{
	*isWictim = false;
	*TypeCheck = SomeType;
	*button = CreateWindow(L"Button", L"Button", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, X, Y,
		Length, Length, hWnd, (HMENU)(X + Y * 10), hInst, nullptr);
	if (*TypeCheck == Checker::Type::WHITE) BMP = L"222.bmp";
	else if (*TypeCheck == Checker::Type::BLACK) BMP = L"111.bmp";
	else BMP = NULL;
	this->Length = Length;
}

Checker::Checker(const Checker& Obj) : isWictim(new bool), TypeCheck(new Type)

{
	*this->isWictim = *Obj.isWictim;
	*this->TypeCheck = *Obj.TypeCheck;
	this->button = Obj.button;
	this->BMP = Obj.BMP;
	this->Length = Obj.Length;
}

const bool& Checker::IsQueen()
{
	return false;
}

const Checker::Type& Checker::GetType() const
{
	return *TypeCheck;
}

Checker::~Checker()
{
	delete isWictim;
	delete TypeCheck;
	delete button;
}

Checker::Checker()
{
}

const bool& Checker::IsWictim() {
	return *isWictim;
}
void Checker::SetType(const Checker::Type& NewType) { *TypeCheck = NewType; }
void Checker::IO_Wictim() { *isWictim = !(*isWictim); }
const HWND& Checker::GetButton() { return *button; }
const wchar_t* Checker::GetBMP() { return BMP; }

void Checker::SearchingForWictims(std::map<Position, Checker*>* BoardPosition, const Position& Pos, const Type& ActivePlayer)
{
	if (Pos.first - 10 > Length && Pos.second > Length)
		CheckIsWictim(Position(Pos.first - Length, Pos.second - Length),
			Position(Pos.first - 2 * Length, Pos.second - 2 * Length), BoardPosition, ActivePlayer);
	if (Pos.first - 10 > Length && Pos.second < 6 * Length)
		CheckIsWictim(Position(Pos.first - Length, Pos.second + Length),
			Position(Pos.first - 2 * Length, Pos.second + 2 * Length), BoardPosition, ActivePlayer);
	if (Pos.first - 10 < 6 * Length && Pos.second > Length)
		CheckIsWictim(Position(Pos.first + Length, Pos.second - Length),
			Position(Pos.first + 2 * Length, Pos.second - 2 * Length), BoardPosition, ActivePlayer);
	if (Pos.first - 10 < 6 * Length && Pos.second < 6 * Length)
		CheckIsWictim(Position(Pos.first + Length, Pos.second + Length),
			Position(Pos.first + 2 * Length, Pos.second + 2 * Length), BoardPosition, ActivePlayer);
}

const bool& Checker::CheckIsWictim(const Position& WictimPos, const Position& NewPos,
	std::map<Position, Checker*>* BoardPosition, const Type& ActivePlayer)
{
	Checker::Type WictimType = (*BoardPosition)[WictimPos]->GetType();
	if ((WictimType == Checker::Type::BLACK ||
		WictimType == Checker::Type::WHITE) &&
		(WictimType != ActivePlayer) &&
		((*BoardPosition)[NewPos]->GetType() == Checker::Type::OPEN) &&
		!(*BoardPosition)[WictimPos]->IsWictim()) // end of IF
	{
		(*BoardPosition)[WictimPos]->IO_Wictim();
		return false;
	}
	return true;
}

const Position& Checker::GetWictimPos(const Turn& turn, std::map<Position, Checker*>* BoardPosition)
{
	return Position((turn.first.first + turn.second.first) / 2, (turn.first.second + turn.second.second) / 2);
}
