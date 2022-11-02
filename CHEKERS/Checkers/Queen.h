#pragma once
#include "Checker.h"

class Queen : public Checker
{
public:
	Queen() {};
	Queen(const Checker& Obj);
	const bool& IsQueen();
	void SearchingForWictims(std::map<Position, Checker*>* BoardPosition, const Position& Pos, const Type& ActivePlayer);
	const Position& GetWictimPos(const Turn& turn, std::map<Position, Checker*>* BoardPosition);

};
