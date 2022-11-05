#pragma once
#include "game_object.h"

class LineObject :
    public GameObject
{
public:
    // crossing line
    LineObject();
    LineObject(glm::vec2 pos, glm::vec2 size,
        Texture2D* lineregularsprite, Texture2D* linediagonalsprite);
    bool isVisible;
    void ShowAt(glm::vec4 StrikeCoords);
private:
    Texture2D* LineRegularSprite;
    Texture2D* LineDiagonalSprite;
    glm::vec2  CheckboardPosition;
    glm::vec2  CheckboardSize;
};

