#include "Queen.h"


Queen::Queen(const Checker& Obj) : Checker(Obj) {
	if (Obj.GetType() == Type::BLACK) BMP = L"111_1.bmp";
	else BMP = L"222_2.bmp";
}

const bool& Queen::IsQueen()
{
	return true;
}

void Queen::SearchingForWictims(std::map<Position, Checker*>* BoardPosition, const Position& Pos, const Type& ActivePlayer)
{
	Position WictimPos(Position(Pos.first - Length, Pos.second - Length));
	Position OpenPos(Position(Pos.first - 2 * Length, Pos.second - 2 * Length));
	while (OpenPos.first > 10 && OpenPos.second > 0)
		if (CheckIsWictim(WictimPos, OpenPos, BoardPosition, ActivePlayer)) {
			WictimPos.first -= Length; WictimPos.second -= Length;
			OpenPos.first -= Length; OpenPos.second -= Length;
		}
	WictimPos = Position(Pos.first - Length, Pos.second + Length);
	OpenPos = Position(Pos.first - 2 * Length, Pos.second + 2 * Length);
	while (OpenPos.first > 10 && OpenPos.second < 8 * Length)
		if (CheckIsWictim(WictimPos, OpenPos, BoardPosition, ActivePlayer)) {
			WictimPos.first -= Length; WictimPos.second += Length;
			OpenPos.first -= Length; OpenPos.second += Length;
		}
	WictimPos = Position(Pos.first + Length, Pos.second - Length);
	OpenPos = Position(Pos.first + 2 * Length, Pos.second - 2 * Length);
	while (OpenPos.first < 8 * Length + 10 && OpenPos.second > 0)
		if (CheckIsWictim(WictimPos, OpenPos, BoardPosition, ActivePlayer)) {
			WictimPos.first += Length; WictimPos.second -= Length;
			OpenPos.first += Length; OpenPos.second -= Length;
		}
	WictimPos = Position(Pos.first + Length, Pos.second + Length);
	OpenPos = Position(Pos.first + 2 * Length, Pos.second + 2 * Length);
	while (OpenPos.first < 8 * Length + 10 && OpenPos.second < 8 * Length)
		if (CheckIsWictim(WictimPos, OpenPos, BoardPosition, ActivePlayer)) {
			WictimPos.first += Length; WictimPos.second += Length;
			OpenPos.first += Length; OpenPos.second += Length;
		}
}

const Position& Queen::GetWictimPos(const Turn& turn, std::map<Position, Checker*>* BoardPosition)
{
	Position WictimPos(turn.first);
	int RotateX, RotateY;
	turn.first.first > turn.second.first ? RotateX = -1 : RotateX = 1;
	turn.first.second > turn.second.second ? RotateY = -1 : RotateY = 1;
	while (!(*BoardPosition)[WictimPos]->IsWictim()) {
		WictimPos.first += RotateX * Length;
		WictimPos.second += RotateY * Length;
	}
	return WictimPos;
}

