#include "game.h"

#include "framework/resource_manager.h"
#include "framework/sprite_renderer.h"
#include <chrono>
#include <thread>
// debug
#include <iostream>

// A little delay between frame to avoid CPU overloading
#define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(5))

// Renderer objects that is responsible for drawing sprites and text
SpriteRenderer* Renderer;
TextRenderer* Text;
TextRenderer* BigText;

// Game objects
std::map<std::string, GameScreen*> GameScreens;
GameScreen* StartScreen;
GameScreen* GameActiveScreen;
GameScreen* GameOverScreen;
GameScreen* CurrentGameScreen;
CheckboardObject* Checkboard;
BotAI* Bot;
Button* StartButton;
Button* RestartButton;
TextCaption* YouVsComputerText;
TextCaption* CurrentCountText;
TextCaption* ResultText;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_MENU), Width(width), Height(height),
    CheckboardSize(glm::vec2(1.0f, 1.0f)), isMouseClicked(false), CurrentMousePos(glm::vec2(0.0f, 0.0f)),
    Victories(0), Defeats(0) { }

Game::~Game()
{
    delete Renderer;
    delete Checkboard;
    delete Bot;
    delete StartScreen;
    delete GameActiveScreen;
    delete GameOverScreen;
    delete StartButton;
    delete RestartButton;
    delete Text;
    delete CurrentCountText;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("res/shaders/sprite.vs", "res/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("res/shaders/text_2d.vs", "res/shaders/text_2d.fs", nullptr, "text");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // create renderers
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Text = new TextRenderer(this->Width, this->Height, ResourceManager::GetShader("text"));
    Text->Load("res/fonts/ocraext.TTF", 30);
    BigText = new TextRenderer(this->Width, this->Height, ResourceManager::GetShader("text"));
    BigText->Load("res/fonts/ocraext.TTF", 56);
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
    
    // create buttons
    glm::vec2 startbuttonsize(Width / 2, Height / 5);
    glm::vec2 startbuttonpos(Width / 2 - startbuttonsize.x / 2, Height / 2 + startbuttonsize.y / 2);
    StartButton = new Button(startbuttonpos, startbuttonsize, ResourceManager::GetTexture("placeholder"), ResourceManager::GetTexture("xcross"));
    
    glm::vec2 restartbuttonsize(Width / 3, Height / 6);
    glm::vec2 restartbuttonpos(Width / 2 + restartbuttonsize.x / 4, checkboardPos.y + CheckboardSize.y - restartbuttonsize.y);
    RestartButton = new Button(restartbuttonpos, restartbuttonsize, ResourceManager::GetTexture("placeholder"), ResourceManager::GetTexture("xcross"));
    
    // create captions
    YouVsComputerText = new TextCaption("You vs Computer", restartbuttonpos.x + restartbuttonsize.x / 2, checkboardPos.y, 1.0f, Text);
    CurrentCountText = new TextCaption("0 : 0", restartbuttonpos.x + restartbuttonsize.x / 2, checkboardPos.y + 50.0f, 1.0f, Text);
    ResultText = new TextCaption("RESULT", restartbuttonpos.x + restartbuttonsize.x / 2, (checkboardPos.y + restartbuttonpos.y + restartbuttonsize.y)/ 2.0f, 1.0f,
        BigText, glm::vec3(0.0f, 0.0f, 0.0f));

    // create start screen
    StartScreen = new GameScreen();
    StartScreen->AddDrawable(StartButton);
    StartScreen->AddInteractive(StartButton);
    GameScreens.insert(std::pair("StartScreen", StartScreen));

    // create main game screen
    GameActiveScreen = new GameScreen();
    GameActiveScreen->AddDrawable(Checkboard);
    GameActiveScreen->AddInteractive(Checkboard);
    GameActiveScreen->AddCaption(YouVsComputerText);
    GameActiveScreen->AddCaption(CurrentCountText);
    GameScreens.insert(std::pair("GameActiveScreen", GameActiveScreen));

    // create game over screen
    GameOverScreen = new GameScreen();
    GameOverScreen->AddDrawable(Checkboard);
    GameOverScreen->AddDrawable(RestartButton);
    GameOverScreen->AddInteractive(RestartButton);
    GameOverScreen->AddCaption(YouVsComputerText);
    GameOverScreen->AddCaption(CurrentCountText);
    GameOverScreen->AddCaption(ResultText);
    GameScreens.insert(std::pair("GameOverScreen", GameOverScreen));

    // set enter screen
    CurrentGameScreen = GameScreens["StartScreen"];
    CurrentGameScreen->SetActiveState(true);
}



void Game::Update(float dt)
{
    if(this->State == PLAYER_MOVE || this->State == BOT_MOVE)
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
             // wait until mouse is released
             if (!isMouseClicked)
             {
                 StartButton->isPressed = false;
                 SwitchToGameScreen(PLAYER_MOVE, "GameActiveScreen");
             }
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
        Bot->MakeMove(); // input is blocked here
    }

    if (this->State == GAME_OVER)
    {
        if (!isMouseClicked) {}
        else
            CurrentGameScreen->HandleInput(CurrentMousePos);
        if (RestartButton->isPressed)
        {
            // wait until mouse is released
            if (!isMouseClicked)
            {
                RestartButton->isPressed = false;
                RestartGame();
            }
        }
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
        CellState result = Checkboard->GetWinner();
        if (result != EMPTY)
            FinishGameWithResult(result);
        
        //if there are no free cells, quit (change state to GAME_OVER)
        if (!Checkboard->isEmptyCellsLeft() && this->State != GAME_OVER)
        {
            FinishGameWithResult(EMPTY);
        }

        Checkboard->onBoardCheckDone();

        // switch player/bot state
        if (this->State == PLAYER_MOVE)
            this->State = BOT_MOVE;
        else if (this->State == BOT_MOVE)
            this->State = PLAYER_MOVE;
    }
}


void Game::RestartGame()
{
    Checkboard->Clear();
    SwitchToGameScreen(PLAYER_MOVE, "GameActiveScreen");
}


void Game::SwitchToGameScreen(GameState newstate, std::string newgamescreenname)
{
    this->State = newstate;
    CurrentGameScreen->SetActiveState(false);
    if (GameScreens[newgamescreenname])
        CurrentGameScreen = GameScreens[newgamescreenname];
    else std::cout << "Couldn't load game screen " << newgamescreenname << ", doesn't exist\n";
    CurrentGameScreen->SetActiveState(true);
}

void Game::FinishGameWithResult(CellState winner)
{
    switch (winner)
    {
    case PLAYER:
    {
        // set capture to win
        ResultText->SetCaption("YOU WIN!");
        // update win\lose stats
        Victories += 1;
        break;
    }
    case BOT:
    {
        // set capture to lose
        ResultText->SetCaption("YOU LOST");
        Defeats += 1;
        break;
    }
    case EMPTY:
    {
        ResultText->SetCaption("DRAW");
    }
    }
    CurrentCountText->SetCaption(std::to_string(Victories) + " : " + std::to_string(Defeats));
    SwitchToGameScreen(GAME_OVER, "GameOverScreen");
}