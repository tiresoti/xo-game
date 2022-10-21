#pragma once
#include "checkboard_object.h"
#include <iostream>

// A bot who is an opponent for a player. Can change checkboard
class BotAI
{
public:
	BotAI(CheckboardObject*);
	void MakeMove();
private:
	CheckboardObject* Checkboard;
	bool isInvincible;
	bool isTwoCellsInARow();
	bool isCentralCellFree();
	bool isClosestFilled();
	void FillAnyFreeCell();
};