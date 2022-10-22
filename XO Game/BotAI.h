#pragma once
#include "checkboard_object.h"
#include <iostream>

// A bot who is an opponent for a player. Changes checkboard object by pointer
class BotAI
{
public:
	BotAI(CheckboardObject*);
	void MakeMove();
private:
	CheckboardObject* Checkboard;
	bool isTwoCellsInARow();
	bool isCentralCellFree();
	bool isClosestFilled();
	void FillAnyFreeCell();
};