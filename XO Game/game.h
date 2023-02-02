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
namespace GameState
{
    enum GameState {
        PLAYER_MOVE,
        BOT_MOVE,
        GAME_MENU,
        GAME_OVER
    };
}


// Game holds all game-related state and functionality.
class Game
{
public:
    Game(unsigned int width, unsigned int height);
    GameState::GameState State;
    // initialize game (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput();
    void Update(float dt);
    void Render();
    // window size doubling option - only call before Init() to avoid cropping
    void SetMouseClicked(bool newstate) { isMouseClicked = newstate; }
    void SetMousePos(glm::vec2&& newposition) { CurrentMousePos = newposition; }
    void DoubleWindowSize();
private:
    // Renderer objects that are responsible for drawing sprites and text
    std::shared_ptr <SpriteRenderer> Renderer;
    std::shared_ptr <TextRenderer> Text;
    std::shared_ptr <TextRenderer> BigText;

    // Game objects
    std::map<std::string, std::shared_ptr <GameScreen>> GameScreens;
    std::shared_ptr <GameScreen> StartScreen;
    std::shared_ptr <GameScreen> GameActiveScreen;
    std::shared_ptr <GameScreen> GameOverScreen;
    std::shared_ptr <GameScreen> CurrentGameScreen;
    std::shared_ptr <GameObject> Logo;
    std::shared_ptr <CheckboardObject> Checkboard;
    std::shared_ptr <BotAI> Bot;
    std::shared_ptr <Button> StartButton;
    std::shared_ptr <Button> RestartButton;
    std::shared_ptr <TextCaption> YouVsComputerText;
    std::shared_ptr <TextCaption> CurrentCountText;
    std::shared_ptr <TextCaption> ResultText;

    unsigned int Width, Height;
    unsigned int Victories, Defeats;
    unsigned int FontScale;

    // mouse stuff
    glm::vec2 CurrentMousePos;
    bool isMouseClicked;

    void RestartGame();
    void CheckBoardChanges();
    void SwitchToGameScreen(GameState::GameState newstate, std::string gamescreenname);
    void FinishGameWithResult(CellState::CellState winner);
};