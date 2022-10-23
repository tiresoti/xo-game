#pragma once
#include "game_object.h"
#include "IMouseInteractive.h"

#include <iostream>

class Button : public GameObject, public IMouseInteractive
{
public:
    Button(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
    bool isPressed;
    void onMouseClick(glm::vec2 MousePosition);
};