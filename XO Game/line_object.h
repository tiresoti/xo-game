#pragma once
#include "game_object.h"

class LineObject :
    public GameObject
{
public:
    // crossing line
    LineObject(glm::vec2 pos, glm::vec2 size,
        Texture2D* lineregularsprite, Texture2D* linediagonalsprite );
private:
    Texture2D* LineRegularSprite;
    Texture2D* LineDiagonalSprite;
};

