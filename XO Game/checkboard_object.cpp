#include "checkboard_object.h"


CheckboardObject::CheckboardObject(glm::vec2 pos, glm::vec2 size, Texture2D boardsprite,
	Texture2D* emptycellsprite, Texture2D* xcellsprite, Texture2D* ocellsprite,
	Texture2D* lineregularsprite, Texture2D* linediagonalsprite
     ) :
	GameObject(pos, size, boardsprite), IMouseInteractive(pos, size),
	EmptyCellSprite(emptycellsprite),
	XCellSprite(xcellsprite), OCellSprite(ocellsprite),
	isBoardChanged(false)
{
	// calculate cell size based on Checkboard size
	float cell_width  = this->Size.x / 3, cell_height  = this->Size.y / 3;
	std::vector<Cell> row;

	// fill the board with empty cells
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
	CrossingLine = LineObject(pos, size, lineregularsprite, linediagonalsprite);
}


void CheckboardObject::onMouseClick(glm::vec2 MousePosition)
{
	int column = GetBoardPart(MousePosition.x, this->Size.x, this->Position.x);
	int row = GetBoardPart(MousePosition.y, this->Size.y, this->Position.y);
	if (Cells[column][row].GetCellState() == CellState::EMPTY)
	{
		Cells[column][row].SetCellState(CellState::PLAYER, XCellSprite);
		isBoardChanged = true;
	}
}

int CheckboardObject::GetBoardPart(float screenPos, float sidelength, float startposition)
{
	// calculates board part based on cursor position
	if (screenPos - startposition <= sidelength / 3)
		return 0; // the left or upper column/row
	if (screenPos - startposition <= 2 * sidelength / 3)
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
			if(cell.GetCellState() != CellState::EMPTY)
			    cell.Draw(renderer);
	if (CrossingLine.isVisible)
		CrossingLine.Draw(renderer);
}

// getter for isBoardChanged
bool CheckboardObject::boardChanged()
{
	return isBoardChanged;
}

void CheckboardObject::onBoardCheckDone()
{
	isBoardChanged = false;
}

// A macro for code readability
#define BoardAt(column, row) Cells[column][row].GetCellState()

// calculate if there is a 3-strike in a row and who owns it
CellState::CellState CheckboardObject::GetWinner()
{
	for (int i = 0; i < 3; i++)
	{
		// if there is a vertical strike...
		if (BoardAt(i, 0) == BoardAt(i, 1) && BoardAt(i, 0) == BoardAt(i, 2)
			&& BoardAt(i, 0) != CellState::EMPTY)
		{
			// set vec2 line coordinates to (i, 0) (i, 2)
			CrossingLine.ShowAt(glm::vec4(i, 0, i, 2));
			return BoardAt(i, 0); // ...return who made it
		}

		// check for a horizontal one
		if (BoardAt(0, i) == BoardAt(1, i) && BoardAt(0, i) == BoardAt(2, i)
			&& BoardAt(0, i) != CellState::EMPTY)
		{
			// set vec2 line coordinates to (0, i) (2, i)
			CrossingLine.ShowAt(glm::vec4(0, i, 2, i));
			return BoardAt(0, i);
		}
	}

	// check if there is a diagonal strike (from left upper to right lower)
	if (BoardAt(0, 0) == BoardAt(1, 1) && BoardAt(0, 0) == BoardAt(2, 2)
		&& BoardAt(1, 1) != CellState::EMPTY)
	{
		// set vec2 line coordinates to (0, 0) (2, 2)
		CrossingLine.ShowAt(glm::vec4(0, 0, 2, 2));
		return BoardAt(1, 1);
	}

	// check if there is a diagonal strike (from left lower to right upper)
	if (BoardAt(0, 2) == BoardAt(1, 1) && BoardAt(0, 2) == BoardAt(2, 0)
		&& BoardAt(1, 1) != CellState::EMPTY)
	{
		// set vec2 line coordinates to (2, 0) (0, 2)
		CrossingLine.ShowAt(glm::vec4(2, 0, 0, 2));
		return BoardAt(1, 1);
	}

	// if there are no strikes, return EMPTY
	return CellState::EMPTY;
}


bool CheckboardObject::isEmptyCellsLeft()
{
	for (std::vector<Cell>& row : this->Cells)
		for (Cell& cell : row)
			if (cell.GetCellState() == CellState::EMPTY)
				return true;
	return false;
}

void CheckboardObject::ChangeCellState(Cell& cell, CellState::CellState newcellstate)
{
	switch (newcellstate)
	{
	case CellState::BOT:
		{
		cell.SetCellState(newcellstate, OCellSprite);
		isBoardChanged = true;
		break;
		}
	case CellState::PLAYER:
	{
		cell.SetCellState(CellState::PLAYER, XCellSprite);
		isBoardChanged = true;
		break;
	}
	case CellState::EMPTY:
	{
		// when we set state to CellState::EMPTY, we don't want the game logic to check if game is over
		cell.SetCellState(CellState::EMPTY, EmptyCellSprite);
		break;
	}
	}
}

std::vector<Cell*> CheckboardObject::GetFreeCellsList()
{
	std::vector<Cell*> freecells;
	for (std::vector<Cell>& row : this->Cells)
		for (Cell& cell : row)
			if (cell.GetCellState() == CellState::EMPTY)
				freecells.push_back(&cell);
	return freecells;
}

void CheckboardObject::Clear()
{
	for (std::vector<Cell>& row : this->Cells)
		for (Cell& cell : row)
			ChangeCellState(cell, CellState::EMPTY);
	CrossingLine.isVisible = false;
}
