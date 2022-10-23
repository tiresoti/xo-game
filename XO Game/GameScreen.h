#pragma once
#include "framework/sprite_renderer.h"
#include "glm/glm.hpp"
#include "game_object.h"
#include "IMouseInteractive.h"
#include <vector>

#include <iostream>

class GameScreen
{
public:
    GameScreen();
    void AddDrawable(GameObject*);
    void AddInteractive(IMouseInteractive*);
    void Draw(SpriteRenderer* Renderer);
    void HandleInput(glm::vec2 MousePosition);
    void SetActiveState(bool activestate);
private:
    std::vector<GameObject*> DrawableElements;
    std::vector <IMouseInteractive*> InteractiveElements;
};

