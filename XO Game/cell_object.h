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
	CellState GetCellState();
	void      SetCellState(CellState newcellstate, Texture2D* newtexture);
	void      Draw(SpriteRenderer& renderer);
private:
	CellState Cellstate;
};
