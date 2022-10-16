#include "cell_object.h"

Cell::Cell(glm::vec2 pos, glm::vec2 size, Texture2D sprite) : GameObject(pos, size, sprite), Cellstate(EMPTY) { }

int Cell::GetCellState()
{
	// returns an int instead of enum, because Game class doesn't know what CellState is
	// but it uses an integer number to calculate result
	return static_cast<int>(Cellstate);
}

int Cell::GetCellNumber(int xScreenPos, int yScreenPos)
{
	// calculates cell based on cursor position
	return 0;
}