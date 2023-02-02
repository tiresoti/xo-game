#include "cell_object.h"

Cell::Cell(glm::vec2 pos, glm::vec2 size, Texture2D sprite) : GameObject(pos, size, sprite), Cellstate(CellState::EMPTY) { }

CellState::CellState Cell::GetCellState()
{
	return this->Cellstate;
}

void Cell::SetCellState(CellState::CellState newcellstate, Texture2D* newtexture)
{
	// we can change state only if it's empty during the game (this->Cellstate == EMPTY)
	// or if we want to clear it (newcellstate == EMPTY)
	if (this->Cellstate == CellState::EMPTY || newcellstate == CellState::EMPTY)
	{
		this->Cellstate = newcellstate;
		this->SetTexture(*newtexture);
	}
}

void Cell::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}