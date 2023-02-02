#pragma once 
  
#include "game_object.h" 
#include "cell_object.h"
#include "line_object.h"
#include "imouse_interactive.h"
#include <vector>
  
class CheckboardObject : public GameObject, public IMouseInteractive
{
public:
    CheckboardObject(glm::vec2 pos, glm::vec2 size,
        Texture2D boardsprite, Texture2D* emptycellsprite,
        Texture2D* xcellsprite, Texture2D* ocellsprite,
        Texture2D* lineregularsprite, Texture2D* linediagonalsprite);
    std::vector<std::vector<Cell>> Cells;
    // input handling (from user or program)
    void        onMouseClick(glm::vec2 MousePosition);
    void        ChangeCellState(Cell& cell, CellState::CellState newcellstate);
    // game logic
    std::vector<Cell*> GetFreeCellsList();
    bool                   boardChanged();
    void                   onBoardCheckDone();
    bool                   isEmptyCellsLeft();
    CellState::CellState   GetWinner();
    void                   Draw(SpriteRenderer& renderer);
    void                   Clear();
private:
    bool        isBoardChanged;
    Texture2D*  EmptyCellSprite;
    Texture2D*  XCellSprite;
    Texture2D*  OCellSprite;
    LineObject  CrossingLine;
    int GetBoardPart(float screenPos, float sidelength, float startposition);
};