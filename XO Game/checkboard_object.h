#pragma once 
  
 #include "game_object.h" 
 #include "cell_object.h"
 #include <vector>
  
class CheckboardObject : public GameObject
{
public:
    CheckboardObject();
    CheckboardObject(glm::vec2 pos, glm::vec2 size, Texture2D boardsprite, Texture2D* emptycellsprite, Texture2D* xcellsprite, Texture2D* ocellsprite);
    std::vector<std::vector<Cell>> Cells;
    // input handling (from user or program)
    void onMouseClick(int xScreenPos, int yScreenPos);
    void ChangeCellState(Cell& cell, CellState newcellstate);
    // game logic
    std::vector<Cell*> GetFreeCellsList();
    bool boardChanged();
    void onBoardCheckDone();
    bool isEmptyCellsLeft();
    int  GetWinner();
    void Draw(SpriteRenderer& renderer);
    // void Clear();
private:
    bool       isBoardChanged;
    Texture2D* EmptyCellSprite;
    Texture2D* XCellSprite;
    Texture2D* OCellSprite;
    int GetBoardPart(int screenPos, float sidelength, float startposition);
};