#pragma once
#include "game_object.h"

class Button : public GameObject
{
    Button(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
    bool isPressed;
    void SetActiveState(bool activestate);
    bool GetActiveState();
private:
    bool isActive;
};