#pragma once
#include "framework/sprite_renderer.h"
#include "glm/glm.hpp"
#include "game_object.h"
#include "imouse_interactive.h"
#include "text_caption.h"
#include <vector>


class GameScreen
{
public:
    GameScreen();
    void AddDrawable(GameObject*);
    void AddInteractive(IMouseInteractive*);
    void AddCaption(TextCaption*);
    void Draw(SpriteRenderer* Renderer);
    void HandleInput(glm::vec2 MousePosition);
    void SetActiveState(bool activestate);
private:
    std::vector <GameObject*>        DrawableElements;
    std::vector <IMouseInteractive*> InteractiveElements;
    std::vector <TextCaption*>       TextElements;
};

