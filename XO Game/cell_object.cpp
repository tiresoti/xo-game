#include "cell_object.h"
#include <iostream>

Cell::Cell(glm::vec2 pos, glm::vec2 size, Texture2D sprite) : GameObject(pos, size, sprite), Cellstate(EMPTY) { }

CellState Cell::GetCellState()
{
	// returns an int instead of enum, because Game class doesn't know what CellState is
	// but it uses an integer number to calculate result
	return this->Cellstate;
}

void Cell::SetCellState(CellState newcellstate, Texture2D* newtexture)
{
	if (this->Cellstate == EMPTY)
	{
		this->Cellstate = newcellstate;
		this->Sprite = *newtexture;
	}
	else std::cout << "Can't change cell state at " << Position.x <<
		" " << Position.y << " - is " << this->Cellstate  << "\n";
}

void Cell::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}