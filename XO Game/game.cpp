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

CheckboardObject* Checkboard;
BotAI* Bot;


Game::Game(unsigned int width, unsigned int height) 
    : State(PLAYER_MOVE), Width(width), Height(height) { }

Game::~Game()
{
    delete Renderer;
    delete Checkboard;
    delete Bot;
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
    //ResourceManager::LoadTexture("res/textures/placeholder.png", true, "placeholder");
    ResourceManager::LoadTexture("res/textures/transparentpixel.png", true, "transparentpixel");

    // initializing input
    isMouseClicked = false;
    CurrentMousePos = glm::vec2(0, 0);

    // create checkboard
    CHECKBOARD_SIZE = glm::vec2(Width / 2.5f, Width / 2.5f);
    glm::vec2 checkboardPos = glm::vec2(
        this->Width / 3.2f - CHECKBOARD_SIZE.x / 2.0f,
        this->Height / 2.0f - CHECKBOARD_SIZE.y / 2.0f
    );
    Checkboard = new CheckboardObject(checkboardPos, CHECKBOARD_SIZE, ResourceManager::GetTexture("checkboard"),
        &ResourceManager::GetTexture("transparentpixel"),
        &ResourceManager::GetTexture("xcross"),
        &ResourceManager::GetTexture("oring"));
    // create bot
    Bot = new BotAI(Checkboard);
    // create captures
    // create buttons
}



void Game::Update(float dt)
{
    CheckBoardChanges();
}

void Game::ProcessInput()
{
    if (this->State == GAME_MENU)
    {
        //
    }

    if (this->State == PLAYER_MOVE)
    {
        if (!isMouseClicked) {} // mouse is usually not clicked
        else                    // if clicked, do the bounds check
            if (isMouseInsideGameObject(Checkboard))
            {
                Checkboard->onMouseClick(CurrentMousePos.x, CurrentMousePos.y);
            }
    }

    if (this->State == BOT_MOVE)
    {
        BotMove();
    }

}

void Game::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
        glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
    );
    Checkboard->Draw(*Renderer);

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
            std::cout << "You won!\n";
            this->State = GAME_OVER;
            break;
        }
        case -1:
        {
            std::cout << "You've lost!\n";
            this->State = GAME_OVER;
            break;
        }
        }

        //if there are no free cells, quit (change state to GAME_OVER)
        if (!Checkboard->isEmptyCellsLeft() && this->State != GAME_OVER)
        {
            std::cout << "Draw!\n";
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