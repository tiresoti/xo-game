/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "game_object.h"
#include "checkboard_object.h"
#include "framework/resource_manager.h"
#include "framework/sprite_renderer.h"
#include <chrono>
#include <thread>
// debug
#include <iostream>


#define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(5))

// A renderer object that is responsible for drawing sprites
SpriteRenderer* Renderer;


CheckboardObject* Checkboard;
GameObject* XCross;
GameObject* ORing;


Game::Game(unsigned int width, unsigned int height) 
    : State(PLAYER_MOVE), Width(width), Height(height) { }

Game::~Game()
{
    delete Renderer;
    delete Checkboard;
    delete XCross;
    delete ORing;
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

    // initializing input
    isMouseClicked = false;
    CurrentMousePos = glm::vec2(0, 0);

    // create checkboard
    CHECKBOARD_SIZE = glm::vec2(Width / 2, Width / 2);
    glm::vec2 checkboardPos = glm::vec2(
        this->Width / 2.0f - CHECKBOARD_SIZE.x / 2.0f,
        this->Height / 2.0f - CHECKBOARD_SIZE.y / 2.0f
    );
    Checkboard = new CheckboardObject(checkboardPos, CHECKBOARD_SIZE, ResourceManager::GetTexture("checkboard"),
        &ResourceManager::GetTexture("placeholder"),
        &ResourceManager::GetTexture("xcross"),
        &ResourceManager::GetTexture("oring"));
    
}



void Game::Update(float dt)
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
        if (!Checkboard->isEmptyCellsLeft())
        {
            std::cout << "Draw!\n";
            this->State = GAME_OVER;
        }

        Checkboard->setBoardChanged(false);

    }
}

void Game::ProcessInput()
{
    if (this->State == PLAYER_MOVE)
    {
        if (!isMouseClicked) {} // mouse is usually not clicked
        else                    // if clicked, do the bounds check
            if (CurrentMousePos.x >= Checkboard->Position.x
                && CurrentMousePos.x <= Checkboard->Position.x + Checkboard->Size.x
                && CurrentMousePos.y >= Checkboard->Position.y
                && CurrentMousePos.y <= Checkboard->Position.y + Checkboard->Size.y)
            {
                Checkboard->onMouseClick(CurrentMousePos.x, CurrentMousePos.y);
            }
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

void Game::Restart()
{}

void BotMove()
{

}