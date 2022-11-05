#pragma once
#include "game_object.h"
#include "IMouseInteractive.h"

class Button : public GameObject, public IMouseInteractive
{
public:
    Button(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
    // a constructor for a button that can change the sprite while pressed
    Button(glm::vec2 pos, glm::vec2 size, Texture2D sprite, Texture2D buttonpressedsprite);
    bool isPressed;
    Texture2D ButtonPressedSprite;
    void onMouseClick(glm::vec2 MousePosition);
    void Draw(SpriteRenderer& renderer);
};