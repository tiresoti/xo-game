/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#pragma once

#include "glad/include/glad/glad.h"
#include "glfw/include/GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>

// Represents the current state of the game
enum GameState {
    PLAYER_MOVE,
    BOT_MOVE,
    GAME_MENU,
    GAME_OVER
};


// Game holds all game-related state and functionality.
class Game
{
public:
    // game state
    GameState               State;
    bool                    isMouseClicked;
    glm::vec2               CurrentMousePos;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput();
    void Update(float dt);
    void Render();
private:
    // initial size of the window
    unsigned int Width, Height;
    // initial size of the checkboard
    glm::vec2 CHECKBOARD_SIZE;
    // reset
    void Restart();
    void CheckBoardChanges();
    void BotMove();
};