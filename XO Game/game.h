#pragma once

#include "game_object.h"
#include "checkboard_object.h"
#include "bot_ai.h"
#include "game_screen.h"
#include "button.h"
#include "text_renderer.h"
#include "text_caption.h"
#include "glad/include/glad/glad.h"
#include "glfw/include/GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "logger.h"
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
    // window size doubling option - only call before Init() to avoid cropping
    void DoubleWindowSize();
private:
    // initial size of the window
    unsigned int Width, Height;
    // win/lose count
    unsigned int Victories, Defeats;
    // font scale
    unsigned int FontScale;
    void RestartGame(); // TODO: game restart
    void CheckBoardChanges();
    void SwitchToGameScreen(GameState newstate, std::string gamescreenname);
    void FinishGameWithResult(CellState winner);
};