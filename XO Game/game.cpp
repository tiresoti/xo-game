#include "game.h"

#include "framework/resource_manager.h"
#include "framework/sprite_renderer.h"
#include <chrono>
#include <thread>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

// A little delay between frame to avoid CPU overloading
#define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(5))


Game::Game(unsigned int width, unsigned int height) 
    : State(GameState::GAME_MENU), Width(width), Height(height),
    isMouseClicked(false), CurrentMousePos(glm::vec2(0.0f, 0.0f)),
    Victories(0), Defeats(0), FontScale(1) { }


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
    Renderer = std::make_shared <SpriteRenderer>(ResourceManager::GetShader("sprite"));
    Text     = std::make_shared <TextRenderer>(this->Width, this->Height, ResourceManager::GetShader("text"));
    Text->Load("res/fonts/ocraext.TTF", 30 * FontScale);
    BigText  = std::make_shared <TextRenderer>(this->Width, this->Height, ResourceManager::GetShader("text"));
    BigText->Load("res/fonts/ocraext.TTF", 56 * FontScale);
    // load textures
    ResourceManager::LoadTexture("res/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("res/textures/checkboard.png", true, "checkboard");
    ResourceManager::LoadTexture("res/textures/xcross.png", true, "xcross");
    ResourceManager::LoadTexture("res/textures/oring.png", true, "oring");
    ResourceManager::LoadTexture("res/textures/placeholder.png", true, "placeholder");
    ResourceManager::LoadTexture("res/textures/startbutton.png", true, "startbutton");
    ResourceManager::LoadTexture("res/textures/startbutton_pressed.png", true, "startbutton_pressed");
    ResourceManager::LoadTexture("res/textures/restartbutton.png", true, "restartbutton");
    ResourceManager::LoadTexture("res/textures/restartbutton_pressed.png", true, "restartbutton_pressed");
    ResourceManager::LoadTexture("res/textures/transparentpixel.png", true, "transparentpixel");
    ResourceManager::LoadTexture("res/textures/line_regular.png", true, "line_regular");
    ResourceManager::LoadTexture("res/textures/line_diagonal.png", true, "line_diagonal");

    // initializing input
    isMouseClicked = false;
    CurrentMousePos = glm::vec2(0, 0);

    // create logo
    glm::vec2 logosize(Width / 3, Width / 3);
    glm::vec2 logopos(Width / 2 - logosize.x / 2, Height / 7);
    //Logo = new GameObject(logopos, logosize, ResourceManager::GetTexture("placeholder"));
    Logo = std::make_shared<GameObject>(logopos, logosize, ResourceManager::GetTexture("placeholder"));
    
    // create checkboard
    glm::vec2 checkboardsize = glm::vec2(Width / 2.5f, Width / 2.5f);
    glm::vec2 checkboardpos = glm::vec2(
        this->Width / 3.2f - checkboardsize.x / 2.0f,
        this->Height / 2.0f - checkboardsize.y / 2.0f
    );
    Checkboard = std::make_shared <CheckboardObject>(checkboardpos, checkboardsize, ResourceManager::GetTexture("checkboard"),
        &ResourceManager::GetTexture("transparentpixel"),
        &ResourceManager::GetTexture("xcross"),
        &ResourceManager::GetTexture("oring"),
        &ResourceManager::GetTexture("line_regular"),
        &ResourceManager::GetTexture("line_diagonal"));

    // create bot
    Bot = std::make_shared<BotAI>(Checkboard);
    
    // create buttons
    glm::vec2 startbuttonsize(Width / 2.5f, Height / 5);
    glm::vec2 startbuttonpos(Width / 2 - startbuttonsize.x / 2, Height / 2 + 5 * startbuttonsize.y / 6);
    StartButton = std::make_shared <Button>(startbuttonpos, startbuttonsize, ResourceManager::GetTexture("startbutton"), ResourceManager::GetTexture("startbutton_pressed"));

    glm::vec2 restartbuttonsize(Width / 3, Height / 6);
    glm::vec2 restartbuttonpos(Width / 2 + restartbuttonsize.x / 4, checkboardpos.y + checkboardsize.y - restartbuttonsize.y);
    RestartButton = std::make_shared <Button>(restartbuttonpos, restartbuttonsize, ResourceManager::GetTexture("restartbutton"), ResourceManager::GetTexture("restartbutton_pressed"));
    
    // create captions
    YouVsComputerText = std::make_shared <TextCaption>("You vs Computer", restartbuttonpos.x + restartbuttonsize.x / 2, checkboardpos.y, 1.0f, Text);
    CurrentCountText  = std::make_shared <TextCaption>("0 : 0", restartbuttonpos.x + restartbuttonsize.x / 2, checkboardpos.y + 50.0f, 1.0f, Text);
    ResultText        = std::make_shared <TextCaption>("RESULT", restartbuttonpos.x + restartbuttonsize.x / 2, (checkboardpos.y + restartbuttonpos.y + restartbuttonsize.y)/ 2.0f, 1.0f,
        BigText, glm::vec3(0.0f, 0.0f, 0.0f));

    // create start screen
    StartScreen = std::make_shared <GameScreen>();
    StartScreen->AddDrawable(Logo);
    StartScreen->AddDrawable(StartButton);
    StartScreen->AddInteractive(StartButton);
    GameScreens.insert(std::pair("StartScreen", StartScreen));

    // create main game screen
    GameActiveScreen = std::make_shared <GameScreen>();
    GameActiveScreen->AddDrawable(Checkboard);
    GameActiveScreen->AddInteractive(Checkboard);
    GameActiveScreen->AddCaption(YouVsComputerText);
    GameActiveScreen->AddCaption(CurrentCountText);
    GameScreens.insert(std::pair("GameActiveScreen", GameActiveScreen));

    // create game over screen
    GameOverScreen = std::make_shared <GameScreen>();
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
    if(this->State == GameState::PLAYER_MOVE || this->State == GameState::BOT_MOVE)
        CheckBoardChanges();
}

void Game::ProcessInput()
{
    if (this->State == GameState::GAME_MENU)
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
                 SwitchToGameScreen(GameState::PLAYER_MOVE, "GameActiveScreen");
             }
         }
    }

    if (this->State == GameState::PLAYER_MOVE)
    {
        if (!isMouseClicked) {}
        else
        {
            CurrentGameScreen->HandleInput(CurrentMousePos);
        }
    }

    if (this->State == GameState::BOT_MOVE)
    {
        Bot->MakeMove(); // input is never handled here
    }

    if (this->State == GameState::GAME_OVER)
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
        CellState::CellState result = Checkboard->GetWinner();
        if (result != CellState::EMPTY)
            FinishGameWithResult(result);
        
        //if there are no free cells, quit (change state to GAME_OVER)
        if (!Checkboard->isEmptyCellsLeft() && this->State != GameState::GAME_OVER)
        {
            FinishGameWithResult(CellState::EMPTY);
        }

        Checkboard->onBoardCheckDone();

        // switch player/bot state
        if (this->State == GameState::PLAYER_MOVE)
            this->State = GameState::BOT_MOVE;
        else if (this->State == GameState::BOT_MOVE)
            this->State = GameState::PLAYER_MOVE;
    }
}


void Game::RestartGame()
{
    Checkboard->Clear();
    SwitchToGameScreen(GameState::PLAYER_MOVE, "GameActiveScreen");
}


void Game::SwitchToGameScreen(GameState::GameState newstate, std::string newgamescreenname)
{
    this->State = newstate;
    CurrentGameScreen->SetActiveState(false);

    if (GameScreens[newgamescreenname])
        CurrentGameScreen = GameScreens[newgamescreenname];
    else
    {
#ifdef DEBUG
        std::cout << "ERROR: Couldn't load game screen " << newgamescreenname << ", doesn't exist\n";
#endif // DEBUG
        Logger::WriteToLog("ERROR: Couldn't load game screen " + newgamescreenname + ", doesn't exist.\n");
        exit(228);
    }
 
    CurrentGameScreen->SetActiveState(true);
}

void Game::FinishGameWithResult(CellState::CellState winner)
{
    switch (winner)
    {
    case CellState::PLAYER:
    {
        // set capture to win
        ResultText->SetCaption("YOU WIN!");
        // update win\lose stats
        Victories += 1;
        break;
    }
    case CellState::BOT:
    {
        ResultText->SetCaption("YOU LOST");
        Defeats += 1;
        break;
    }
    case CellState::EMPTY:
    {
        ResultText->SetCaption("DRAW");
    }
    }
    CurrentCountText->SetCaption(std::to_string(Victories) + " : " + std::to_string(Defeats));
    SwitchToGameScreen(GameState::GAME_OVER, "GameOverScreen");
}

void Game::DoubleWindowSize()
{
    this->Width *= 2;
    this->Height *= 2;
    FontScale *= 2;
}