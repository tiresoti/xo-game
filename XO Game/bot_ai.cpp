#include "bot_ai.h"
#include <random>
#include <algorithm>
#include <chrono>

BotAI::BotAI(std::shared_ptr <CheckboardObject> checkboard) :  Checkboard(checkboard) { }


void BotAI::MakeMove()
{
    // firstly we check if we're about to lose - a condition of having 1 cell left for a strike
    if (isTwoCellsInARow())
        return;
    // then we try to fill the central cell if not filled yet
    if (isCentralCellFree())
        return;
    // if there are free upper/lower/left/right, we fill them
    if (isClosestFilled())
        return;
    FillAnyFreeCell();
}

// a little macro magic for representing cell states as integers and computing the move
#define BoardAt(column, row) static_cast<int>(Checkboard->Cells[column][row].GetCellState())

bool BotAI::isTwoCellsInARow()
{
    // check columns for having two cells filled by player or bot
    for (int i = 0; i < 3; i++)
    {
        // if there are 2 cells filled by player or bot in a row and one is empty,
        // the sum of 3 cells is either 2 or -2. this hack is used here
        if (abs(BoardAt(i, 0) + BoardAt(i, 1) + BoardAt(i, 2)) == 2)
            for (int j = 0; j < 3; j++)
                if (BoardAt(i, j) == 0)
                {
                    Checkboard->ChangeCellState(Checkboard->Cells[i][j], CellState::BOT);
                    return true;
                }
    }

    // check rows
    for (int i = 0; i < 3; i++)
    {
        if (abs(BoardAt(0, i) + BoardAt(1, i) + BoardAt(2, i)) == 2)
            for (int j = 0; j < 3; j++)
                if (BoardAt(j, i) == 0)
                {
                    Checkboard->ChangeCellState(Checkboard->Cells[j][i], CellState::BOT);
                    return true;
                }
    }

    // check 2 diagonals
    if (abs(BoardAt(0, 0) + BoardAt(1, 1) + BoardAt(2, 2)) == 2)
        for (int i = 0, j = 0; j < 3; i++, j++)
            if (BoardAt(i, j) == 0)
            {
                Checkboard->ChangeCellState(Checkboard->Cells[i][j], CellState::BOT);
                return true;
            }

    if (abs(BoardAt(2, 0) + BoardAt(1, 1) + BoardAt(0, 2)) == 2)
    {
        if (BoardAt(2, 0) == 0)
        {
            Checkboard->ChangeCellState(Checkboard->Cells[2][0], CellState::BOT);
            return true;
        }
        if (BoardAt(1, 1) == 0)
        {
            Checkboard->ChangeCellState(Checkboard->Cells[1][1], CellState::BOT);
            return true;
        }
        if (BoardAt(0, 2) == 0)
        {
            Checkboard->ChangeCellState(Checkboard->Cells[0][2], CellState::BOT);
            return true;
        }
    }
        
    return false;
}

bool BotAI::isCentralCellFree()
{
    if (BoardAt(1, 1) == 0)
    {
        Checkboard->ChangeCellState(Checkboard->Cells[1][1], CellState::BOT);
        return true;
    }
    return false;
}
bool BotAI::isClosestFilled()
{
    // firstly we find any bot-filled cell coordinates
    int x = -1;
    int y = -1;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(BoardAt(i, j) == -1)
            {
                x = i;
                y = j;
                break;
            }
    // if there are none, return
    if (x < 0 || y < 0)
        return false;

    // then we fill a random nearby cell from list of nearby cells
    std::vector <Cell*> nearestcells;
    if (BoardAt(std::clamp(x + 1, 0, 2), y) == 0)
        nearestcells.push_back(&Checkboard->Cells[std::clamp(x + 1, 0, 2)][y]);
    if (BoardAt(std::clamp(x - 1, 0, 2), y) == 0)
        nearestcells.push_back(&Checkboard->Cells[std::clamp(x - 1, 0, 2)][y]);
    if (BoardAt(x, std::clamp(y + 1, 0, 2)) == 0)
        nearestcells.push_back(&Checkboard->Cells[x][std::clamp(y + 1, 0, 2)]);
    if (BoardAt(x, std::clamp(y - 1, 0, 2)) == 0)
        nearestcells.push_back(&Checkboard->Cells[x][std::clamp(y - 1, 0, 2)]);

    srand((unsigned)time(NULL));
    if (nearestcells.size() > 0)
    {
        Checkboard->ChangeCellState(*nearestcells[rand() % nearestcells.size()], CellState::BOT);
        return true;
    }
    return false;
}

void BotAI::FillAnyFreeCell()
{
    // get a list of free cells
    std::vector <Cell*> freecells = Checkboard->GetFreeCellsList();
    // fill a random cell from list
    srand((unsigned)time(NULL));
    Checkboard->ChangeCellState(*freecells[rand() % freecells.size()], CellState::BOT);
}