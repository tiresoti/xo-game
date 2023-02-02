#pragma once
#include "checkboard_object.h"


// A bot who is an opponent for a player. Changes checkboard object by pointer
class BotAI
{
public:
	BotAI(std::shared_ptr <CheckboardObject>);
	void MakeMove();
private:
	std::shared_ptr <CheckboardObject> Checkboard;
	bool isTwoCellsInARow();
	bool isCentralCellFree();
	bool isClosestFilled();
	void FillAnyFreeCell();
};