#include "checkboard_object.h"

#include <iostream>


CheckboardObject::CheckboardObject()
	: GameObject(), EmptyCellSprite(), XCellSprite(), OCellSprite() { }

CheckboardObject::CheckboardObject(glm::vec2 pos, glm::vec2 size, Texture2D boardsprite, Texture2D* emptycellsprite, Texture2D* xcellsprite, Texture2D* ocellsprite) : GameObject(pos, size, boardsprite), EmptyCellSprite(emptycellsprite), XCellSprite(xcellsprite), OCellSprite(ocellsprite)
{
	// calculate cell size based on Checkboard size
	float cell_width  = this->Size.x / 3, cell_height  = this->Size.y / 3;
	std::vector<Cell> row;

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			glm::vec2 pos(cell_width * x + this->Position.x, cell_height * y + this->Position.y);
			glm::vec2 size(cell_width, cell_height);
			Cell cell = Cell(pos, size, *EmptyCellSprite);
			row.push_back(cell);
		}
		Cells.push_back(row);
		row.clear();
	}
}


void CheckboardObject::onMouseClick(int xScreenPos, int yScreenPos)
{
	int column = GetBoardPart(xScreenPos, this->Size.x, this->Position.x);
	int row = GetBoardPart(yScreenPos, this->Size.y, this->Position.y);
	Cells[column][row].SetCellState(PLAYER, XCellSprite);
}

int CheckboardObject::GetBoardPart(int screenPos, float sidelength, float startposition)
{
	// calculates board part based on cursor position
	if (static_cast<float>(screenPos) - startposition <= sidelength / 3)
		return 0; // the left or upper column/row
	if (static_cast<float>(screenPos) - startposition <= 2 * sidelength / 3)
		return 1; // the middle column/row
	return 2 ;// the right or lower column/row
}

void CheckboardObject::Draw(SpriteRenderer& renderer)
{
	// draw the board itself
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
	// draw cells
	for (std::vector<Cell>& row : this->Cells)
		for (Cell& cell : row)
			if(cell.GetCellState() != EMPTY) // TODO: remove empty sprite
			    cell.Draw(renderer);

}