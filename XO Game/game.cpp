#include "game.h"

#include "framework/resource_manager.h"
#include "framework/sprite_renderer.h"
#include <chrono>
#include <thread>
// debug
#include <iostream>

// A little delay between frame to avoid CPU overloading
#define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(5))

// A renderer object that is responsible for drawing sprites
SpriteRenderer* Renderer;
// Game objects
std::map<std::string, GameScreen*> GameScreens;
GameScreen* StartScreen;
GameScreen* GameActiveScreen;
GameScreen* GameOverScreen;
GameScreen* CurrentGameScreen;
CheckboardObject* Checkboard;
BotAI* Bot;
Button* StartButton;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_MENU), Width(width), Height(height),
    CheckboardSize(glm::vec2(1.0f, 1.0f)), isMouseClicked(false), CurrentMousePos(glm::vec2(0.0f, 0.0f)) { }

Game::~Game()
{
    delete Renderer;
    delete Checkboard;
    delete Bot;
    delete StartScreen;
    delete GameActiveScreen;
    delete GameOverScreen;
    delete StartButton;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("res/shaders/sprite.vs", "res/shaders/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("res/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("res/textures/checkboard.png", true, "checkboard");
    ResourceManager::LoadTexture("res/textures/xcross.png", true, "xcross");
    ResourceManager::LoadTexture("res/textures/oring.png", true, "oring");
    ResourceManager::LoadTexture("res/textures/placeholder.png", true, "placeholder");
    ResourceManager::LoadTexture("res/textures/transparentpixel.png", true, "transparentpixel");

    // initializing input
    isMouseClicked = false;
    CurrentMousePos = glm::vec2(0, 0);

    // create checkboard
    CheckboardSize = glm::vec2(Width / 2.5f, Width / 2.5f);
    glm::vec2 checkboardPos = glm::vec2(
        this->Width / 3.2f - CheckboardSize.x / 2.0f,
        this->Height / 2.0f - CheckboardSize.y / 2.0f
    );
    Checkboard = new CheckboardObject(checkboardPos, CheckboardSize, ResourceManager::GetTexture("checkboard"),
        &ResourceManager::GetTexture("transparentpixel"),
        &ResourceManager::GetTexture("xcross"),
        &ResourceManager::GetTexture("oring"));
    // create bot
    Bot = new BotAI(Checkboard);
    // create captures
    // create buttons
    glm::vec2 startbuttonsize(Width / 2, Height / 5);
    glm::vec2 startbuttonpos(Width / 2 - startbuttonsize.x / 2, Height / 2 + startbuttonsize.y / 2);
    StartButton = new Button(startbuttonpos, startbuttonsize, ResourceManager::GetTexture("placeholder"));
    
    // create start screen
    StartScreen = new GameScreen();
    StartScreen->AddDrawable(StartButton);
    StartScreen->AddInteractive(StartButton);
    GameScreens.insert(std::pair("StartScreen", StartScreen));

    // create main game screen
    GameActiveScreen = new GameScreen();
    GameActiveScreen->AddDrawable(Checkboard);
    GameActiveScreen->AddInteractive(Checkboard);
    GameScreens.insert(std::pair("GameActiveScreen", GameActiveScreen));

    // create game over screen
    GameOverScreen = new GameScreen();
    GameScreens.insert(std::pair("GameOverScreen", GameOverScreen));

    // set enter screen
    CurrentGameScreen = GameScreens["StartScreen"];
    CurrentGameScreen->SetActiveState(true);
}



void Game::Update(float dt)
{
    //if(this->State == PLAYER_MOVE || this->State == BOT_MOVE)
        CheckBoardChanges();
}

void Game::ProcessInput()
{
    if (this->State == GAME_MENU)
    {
         if (!isMouseClicked) {} // mouse is usually not clicked
         else                    // if clicked, handle input
         {
             CurrentGameScreen->HandleInput(CurrentMousePos);
         }

         if (StartButton->isPressed)
         {
             // TODO: create method SwitchToGameScreen(std::string gamescreenname)
             StartButton->isPressed = false;
             this->State = PLAYER_MOVE;
             CurrentGameScreen->SetActiveState(false);
             CurrentGameScreen = GameScreens["GameActiveScreen"];
             CurrentGameScreen->SetActiveState(true);
         }
    }

    if (this->State == PLAYER_MOVE)
    {
        if (!isMouseClicked) {}
        else
            CurrentGameScreen->HandleInput(CurrentMousePos);
    }

    if (this->State == BOT_MOVE)
    {
        BotMove();
    }

    if (this->State == GAME_OVER)
    {
        
    }

}

void Game::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
        glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
    );
    CurrentGameScreen->Draw(Renderer);

    SLEEP;
}


void Game::CheckBoardChanges()
{
    if (Checkboard->boardChanged())
    {
        int result = Checkboard->GetWinner();
        switch (result)
        {
        case 1:
        {
            std::cout << "You won!\n"; // set capture to win
            // call method SwitchToGameScreen(std::string gamescreenname) instead
            this->State = GAME_OVER;
            break;
        }
        case -1:
        {
            std::cout << "You've lost!\n"; // set capture to lose
            // call method SwitchToGameScreen(std::string gamescreenname) instead
            this->State = GAME_OVER;
            break;
        }
        }

        //if there are no free cells, quit (change state to GAME_OVER)
        if (!Checkboard->isEmptyCellsLeft() && this->State != GAME_OVER)
        {
            std::cout << "Draw!\n"; // set capture to draw
            // call method SwitchToGameScreen(std::string gamescreenname) instead
            this->State = GAME_OVER;
        }

        Checkboard->onBoardCheckDone();

        // switch player/bot state
        if (this->State == PLAYER_MOVE)
            this->State = BOT_MOVE;
        else if (this->State == BOT_MOVE)
            this->State = PLAYER_MOVE;
    }
}


void Game::BotMove()
{
    Bot->MakeMove();
}

void Game::Restart()
{}

bool Game::isMouseInsideGameObject(GameObject* gameobject)
{
    return (CurrentMousePos.x >= gameobject->Position.x
        && CurrentMousePos.x <= gameobject->Position.x + gameobject->Size.x
        && CurrentMousePos.y >= gameobject->Position.y
        && CurrentMousePos.y <= gameobject->Position.y + gameobject->Size.y
        );
}