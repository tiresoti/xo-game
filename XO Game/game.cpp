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
#include "framework/resource_manager.h"
#include "framework/sprite_renderer.h"
#include <chrono>
#include <thread>

#define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(5))

// A renderer object that is responsible for drawing sprites
SpriteRenderer* Renderer;

// Initial size of the player paddle
const glm::vec2 CHECKBOARD_SIZE(400.0f, 400.0f);
const glm::vec2 CELL_SIZE(100.0f, 100.0f);

GameObject* Checkboard;
GameObject* XCross;
GameObject* ORing;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Width(width), Height(height) { }

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

    // create a list of
    // create checkboard
    glm::vec2 checkboardPos = glm::vec2(
        this->Width / 2.0f - CHECKBOARD_SIZE.x / 2.0f,
        this->Height / 2.0f - CHECKBOARD_SIZE.y / 2.0f
    );
    Checkboard = new GameObject(checkboardPos, CHECKBOARD_SIZE, ResourceManager::GetTexture("checkboard"));
    
    // temporary
    glm::vec2 XCrossPos = glm::vec2(
        this->Width / 3.0f - CELL_SIZE.x / 2.0f,
        this->Height / 3.0f - CELL_SIZE.y / 2.0f
    );
    glm::vec2 ORingPos = glm::vec2(
        this->Width / 6.0f - CELL_SIZE.x / 2.0f,
        this->Height / 6.0f - CELL_SIZE.y / 2.0f
    );
    XCross = new GameObject(XCrossPos, CELL_SIZE, ResourceManager::GetTexture("xcross"));
    ORing = new GameObject(ORingPos, CELL_SIZE, ResourceManager::GetTexture("oring"));
    ORing->SetTexture(ResourceManager::GetTexture("xcross"));
}



void Game::Update(float dt)
{
    //Ball->Move(dt, this->Width);
    //// check for collisions
    //this->DoCollisions();
    //if (Ball->Position.y >= this->Height) // did ball reach bottom edge?
    //{
    //    this->ResetLevel();
    //    this->ResetPlayer();
    //}
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"),
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
        );
        Checkboard->Draw(*Renderer);
        XCross->Draw(*Renderer);
        ORing->Draw(*Renderer);
    }
    SLEEP;
}


void Game::DoCollisions()
{

}

void Game::ResetLevel()
{}

void Game::ResetPlayer()
{
    // reset player/ball stats
    //Checkboard->Size = CHECKBOARD_SIZE;
    //Checkboard->Position = glm::vec2(this->Width / 2.0f - CHECKBOARD_SIZE.x / 2.0f, this->Height - CHECKBOARD_SIZE.y);
}