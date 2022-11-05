#include "line_object.h"

LineObject::LineObject(): LineRegularSprite(), LineDiagonalSprite(), isVisible(false),
CheckboardPosition(glm::vec2(0,0)), CheckboardSize(glm::vec2(0, 0)) { }

LineObject::LineObject(glm::vec2 pos, glm::vec2 size,
    Texture2D* lineregularsprite, Texture2D* linediagonalsprite) :
    GameObject(pos, size, *lineregularsprite),
    LineRegularSprite(lineregularsprite), LineDiagonalSprite(linediagonalsprite), isVisible(false),
    CheckboardPosition(pos), CheckboardSize(size) { }

// updates line position and makes it visible
void LineObject::ShowAt(glm::vec4 StrikeCoords)
{
    // line is horizontal if y1 = y2 and x1 != x2
    // StrikeCoords x1, x2, y1, y2 are x, y, z, w
    if (StrikeCoords.y == StrikeCoords.w && StrikeCoords.x != StrikeCoords.z)
    {
        this->Position.x = CheckboardPosition.x;
        this->Position.y = (StrikeCoords.y + 1) * CheckboardSize.y / 3;
        this->Sprite = *this->LineRegularSprite;
        this->Size = glm::vec2(CheckboardSize.x, CheckboardSize.y / 3);
        this->Rotation = 0.0f;
    }
    // line is vertical if x1 = x2 and y1 != y2
    else if (StrikeCoords.x == StrikeCoords.z && StrikeCoords.y != StrikeCoords.w)
    {
        // x position needed a little offset idk why
        this->Position.x = (StrikeCoords.x + 2) * CheckboardSize.x / 3 - CheckboardSize.x / 24;
        this->Position.y = CheckboardPosition.y;
        this->Sprite = *this->LineRegularSprite;
        this->Size = glm::vec2(CheckboardSize.x, CheckboardSize.y / 3);
        this->Rotation = 90.0f;
    }
    // diagonal line from upper left to lower right
    else if (StrikeCoords.x != StrikeCoords.z && StrikeCoords.y != StrikeCoords.w
        && StrikeCoords.x == 0)
    {
        this->Position.x = CheckboardPosition.x;
        this->Position.y = CheckboardPosition.y;
        this->Sprite = *this->LineDiagonalSprite;
        this->Size = glm::vec2(CheckboardSize.x, CheckboardSize.y);
        this->Rotation = 0.0f;
    }
    // diagonal line from lower left to upper right
    else if (StrikeCoords.x != StrikeCoords.z && StrikeCoords.y != StrikeCoords.w
        && StrikeCoords.x == 2)
    {
        this->Position.x = CheckboardPosition.x + CheckboardSize.x;
        this->Position.y = CheckboardPosition.y;
        this->Sprite = *this->LineDiagonalSprite;
        this->Size = glm::vec2(CheckboardSize.x, CheckboardSize.y);
        this->Rotation = 90.0f;
    }
    this->isVisible = true;
}
