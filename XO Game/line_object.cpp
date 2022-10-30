#include "line_object.h"

LineObject::LineObject(glm::vec2 pos, glm::vec2 size,
    Texture2D* lineregularsprite, Texture2D* linediagonalsprite) :
    GameObject(pos, size, *lineregularsprite),
    LineRegularSprite(lineregularsprite), LineDiagonalSprite(linediagonalsprite)
{ }

//
