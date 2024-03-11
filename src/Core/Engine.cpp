#include <iostream>
#include "../Graphics/TextureManager.h"
#include "Engine.h"
#include "../Characters/Sprites.h"
#include "../Characters/Monster.h"
#include "../Inputs/Input.h"
#include "../Timer/timer.h"
#include "../Camera/Camera.h"
#include "../map/MapParser.h"

Engine *Engine::s_Instance = nullptr;
Sprites* player = nullptr;
Monster* enemy = nullptr;

bool Engine::Init() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) return false;

    m_Window = SDL_CreateWindow("Advanture Time", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Log("Success Init !!!\n");

    player = new Sprites(new Properties("player-idle", 600, 600, 64, 40, 5));

    player->Load("player-idle", "assets/player/Idle/Idle Sword", 5);
    player->Load("player-attack", "assets/player/Player_Attack/Attack", 3);
    player->Load("player-run", "assets/player/Run/Run Sword", 6);
    player->Load("player-jump", "assets/player/Jump/Jump Sword", 3);

    if(!MapParser::GetInstance()->Load()) {
        std::cerr << "Failed to Load Map\n";
    }

    m_LevelMap = MapParser::GetInstance()->GetMap("LEVEL_1");

    // enemy = new Monster(new Properties("monster", 100, 500, 34, 30, 5, "assets/enemy/01-Idle/Idle-"));

    Camera::GetInstance()->SetTarget(player->GetOrigin());

    return m_isRunning = true;
}

void Engine::Update() {
    double dt = Timer::getInstance()->getDeltaTime();
    player->Update(dt);
    m_LevelMap->Update();
    Camera::GetInstance()->Update(dt);
    // enemy->Update(0);
}

bool Engine::Clean() {
    TextureManager::GetInstance()->Clean();
    SDL_DestroyWindow(m_Window);
    SDL_DestroyRenderer(m_Renderer);
    IMG_Quit();
    SDL_Quit();
    SDL_Log("Success Clean !!!\n");
}

void Engine::Render() {
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);
    // enemy->Draw();
    m_LevelMap->Render();
    player->Draw();
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
    Input::getInstance()->Listen();
}

void Engine::Quit() {
    m_isRunning = false;
}