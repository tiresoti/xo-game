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
    void AddDrawable(std::shared_ptr <GameObject>);
    void AddInteractive(std::shared_ptr <IMouseInteractive>);
    void AddCaption(std::shared_ptr <TextCaption>);
    void Draw(std::shared_ptr <SpriteRenderer> Renderer);
    void HandleInput(glm::vec2 MousePosition);
    void SetActiveState(bool activestate);
private:
    std::vector <std::shared_ptr<GameObject>>        DrawableElements;
    std::vector <std::shared_ptr <IMouseInteractive>> InteractiveElements;
    std::vector <std::shared_ptr <TextCaption>>       TextElements;
};

