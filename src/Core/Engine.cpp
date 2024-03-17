#include <iostream>
#include "../Graphics/TextureManager.h"
#include "Engine.h"
#include "../Characters/Sprites.h"
#include "../Characters/Monster.h"
#include "../Inputs/Input.h"
#include "../Timer/timer.h"
#include "../Camera/Camera.h"
#include "../map/MapParser.h"
#include "../Characters/Object.h"
#include "../Characters/item.h"
#include "../Characters/effect.h"
#include "../Inputs/Mouse.h"

Engine *Engine::s_Instance = nullptr;
Sprites* player = nullptr;
Effect* player_attack_effected = nullptr;
Mix_Chunk* gameMusic;
Mix_Chunk* gameOver;

std::vector<Object*> box;
std::vector<Item*> coin;
std::vector<Monster*> enemy;
std::vector<Item*> heal;
std::vector<Object*> portal_gate;

const int NUM_OF_HEAL = 3;

std::vector<std::pair<int, int>> postision_box[3] = {
     { {6, 15}, {15, 15} },
     {  },
     { }
};
std::vector<std::pair<int, int>> sliver_postision_item[3] = {
    { {9, 15}, {11 , 15}, {13, 17}, {22, 14}, {28, 14}, {35, 12}, {48, 15}, {55, 13}, {67, 7}},
    { },
    { }
};
std::vector<std::pair<int, int>> enemy_postision[3] = {
    {},
    { },
    { }
};
std::vector<std::pair<int, int>> gold_postision_item[3] = {
    { {2, 16} , {31, 16}, {69, 6}, {40, 16}},
    { },
    { }
};
std::vector<std::pair<int, int>> portal_gate_postision[3] = {
    { {69, 9} },
    {  },
    {  } 
};

std::vector<std::pair<int, int>> teleport_gate[3] = {
    {},
    {},
    {}
};

bool Engine::Init() {
    // init SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) return false;
    m_Window = SDL_CreateWindow("Advanture Time", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Log("Success Init !!!\n");

    // init Mix
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 128);
    gameMusic = Mix_LoadWAV("assets/sounds/music_game.wav");
    Mix_PlayChannel(-1, gameMusic, -1);
    gameOver = Mix_LoadWAV("assets/sounds/round_end.wav");

    // create player
    player = new Sprites(new Properties("player-idle", 8 * 80, 15 * 80, 64, 40, 5));
    player->Load("player-idle", "assets/player/Idle/Idle Sword", 5);
    player->Load("player-attack", "assets/player/Player_Attack/Attack", 3);
    player->Load("player-run", "assets/player/Run/Run Sword", 6);
    player->Load("player-jump", "assets/player/Jump/Jump Sword", 3);
    player->Load("player-dead", "assets/player/Dead/Dead Hit", 4);
    player->Load("player-next-level", "assets/player/06-Hit Sword/Hit Sword", 4);

    // attack_effected;
    player_attack_effected = new Effect(new Properties("player-attack-effected", 600, 600, 64, 40, 5));
    player_attack_effected->Load("player-attack-effect", "assets/player/player_attack_effect/Attack", 3);

    // heal
    heal.clear();
    for(int i = 0; i < NUM_OF_HEAL; i++) {
        Item* new_heal = new Item(new Properties("heal-full", 17 * 80 + i * 80, 10 * 80, 9, 9, 8));
        new_heal->Load("heal-full", "assets/ui/full_blinking", 1);
        new_heal->setType(1);
        heal.push_back(new_heal);
    }

    // origin item
    player->addEffect(player_attack_effected);
    player->addHeal(heal);

    // map create
    MapParser::GetInstance()->Load();
    m_LevelMap = MapParser::GetInstance()->GetMap("GAME_MAP");

    Mouse::getInstance()->Load("mouse", "assets/inputs/slick_arrow-arrow.png");

    return m_isRunning = true;
}

void Engine::Init_Level(int level) {
    // if(level == 2) {
    //     player->setSpawnPosition(4 * 80, 16 * 80);
    // }

    // create box
    box.clear();
    for(auto pos : postision_box[level - 1]) {
        Object* new_box = new Object(new Properties("box-idle", pos.first * 80, pos.second * 80, 28, 22, 5));
        new_box->Load("box-idle", "assets/Objects/Box/Idle/idle", 1);
        new_box->Load("box-hit", "assets/Objects/Box/Hit/hit", 4);
        new_box->Load("box-destroy", "assets/Objects/Box/Destroyed/destroy", 5);
        new_box->setType(0);
        box.push_back(new_box);
    }

    // create coin
    coin.clear();
    for(auto pos : sliver_postision_item[level - 1]) {
        Item* new_coin = new Item(new Properties("sliver-coin", pos.first * 80, pos.second * 80, 16, 16, 5));
        new_coin->Load("sliver-coin", "assets/Objects/Silver Coin/sliver-coin", 4);
        new_coin->Load("coin-effect", "assets/Objects/Coin Effect/coin-effect", 3);
        new_coin->setType(0);
        coin.push_back(new_coin);
    }

    for(auto pos : gold_postision_item[level - 1]) {
        Item* new_coin = new Item(new Properties("gold-coin", pos.first * 80, pos.second * 80, 16, 16, 5));
        new_coin->Load("gold-coin", "assets/Objects/Gold Coin/gold-coin", 4);
        new_coin->Load("coin-effect", "assets/Objects/Coin Effect/coin-effect", 3);
        new_coin->setType(2);
        coin.push_back(new_coin);
    }

    // create portal gate
    for(auto pos : portal_gate_postision[level - 1]) {
        Object* new_portal_gate = new Object(new Properties("portal-gate", pos.first * 80, pos.second * 80, 20, 32, 10));
        new_portal_gate->Load("portal-gate", "assets/Objects/portal-gate/pixil-frame", 6);
        new_portal_gate->setType(1);
        portal_gate.push_back(new_portal_gate);
    }

    // create enemy respawn
    enemy.clear();
    for(auto pos : enemy_postision[level - 1]) {
        Monster* new_enemy = new Monster(new Properties("monster", pos.first * 80, pos.second * 80, 34, 30, 5));
        new_enemy->Load("enemy-idle", "assets/enemy/01-Idle/Idle", 8);
        new_enemy->Load("enemy-run", "assets/enemy/02-Run/Run", 6);
        new_enemy->Load("enemy-hit", "assets/enemy/05-Hit/hit", 4);
        new_enemy->Load("enemy-dead", "assets/enemy/06-Dead Hit/enemy-hit", 4);
        new_enemy->Load("enemy-deaded", "assets/enemy/07-Dead Ground/dead", 4);
        enemy.push_back(new_enemy);
    }

    // add item
    player->addBox(box);
    player->addItem(coin);
    player->addEnemy(enemy);
    player->addGateLevel(portal_gate[0]);

    // camera 
    Camera::GetInstance()->SetTarget(player->GetOrigin());
}

void Engine::Update() {
    for(int LEVEL = 1; LEVEL <= 3; LEVEL++) {
        if(player->getLevel() == LEVEL && !loaded_level[LEVEL - 1]) {
            Init_Level(LEVEL);
            loaded_level[LEVEL - 1] = true;
            cur_level = LEVEL;
        }
    }
    double dt = Timer::getInstance()->getDeltaTime();
    player->Update(dt);
    player_attack_effected->Update(dt);
    if(player->getTurn() > NUM_OF_HEAL) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game Over", "You Lose", NULL);
        Quit();
    }   
    m_LevelMap->Update();
    for(auto it = heal.begin(); it != heal.end(); /* no increment here */) {
        if((*it)->isToBeDestroyed()) {
            it = heal.erase(it);
        } else {
            ++it;
        }
    }
    for(auto it = box.begin(); it != box.end(); /* no increment here */) {
        if((*it)->isToBeDestroyed()) {
            it = box.erase(it);
        } else {
            ++it;
        }
    }
    for(auto it = coin.begin(); it != coin.end(); /* no increment here */) {
        if((*it)->isToBeDestroyed()) {
            it = coin.erase(it);
        } else {
            ++it;
        }
    }
    for(auto t : enemy) {
        t->Update(dt);
    }
    for(auto t : box) {
        t->Update(dt);
    }
    for(auto t : coin) {
        t->Update(dt);
    }
    for(auto t : heal) {
        t->Update(dt);
    }
    for(auto t : portal_gate) {
        t->Update(dt);
    }
    Mouse::getInstance()->Update();
    Camera::GetInstance()->Update(dt);
}

bool Engine::Clean() {
    TextureManager::GetInstance()->Clean();
    SDL_DestroyWindow(m_Window);
    SDL_DestroyRenderer(m_Renderer);
    MapParser::GetInstance()->Clean();
    IMG_Quit();
    Mix_FreeChunk(gameMusic);
    Mix_FreeChunk(gameOver);
    Mix_Quit();
    SDL_Quit();
    SDL_Log("Success Clean !!!\n");
}

void Engine::Render() {
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);
    m_LevelMap->Render();
    player->Draw();
    for(auto t : enemy) {
        t->Draw();
    }
    player_attack_effected->Draw();
    for(auto t : box) {
        t->Draw();
    }
    for(auto t : coin) {
        t->Draw();
    }
    for(auto t : heal) {
        t->Draw();
    }
    for(auto t : portal_gate) {
        t->Draw();
    }
    Mouse::getInstance()->Draw();
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
    Input::getInstance()->Listen();
}

void Engine::Quit() {
    m_isRunning = false;
}