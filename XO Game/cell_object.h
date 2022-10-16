#pragma once
#include "game_object.h"

enum CellState
{
	BOT    = -1,
	EMPTY  =  0,
	PLAYER =  1
};

class Cell : public GameObject
{
public:
	Cell(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
	int GetCellState();
	int GetCellNumber(int xScreenPos, int yScreenPos);
private:
	CellState Cellstate;
};
