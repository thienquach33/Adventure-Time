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
#include "../Inputs/Button.h"
#include "../Characters/Decor.h"

Engine *Engine::s_Instance = nullptr;
Sprites* player = nullptr;
Mix_Chunk* gameMusic;
Mix_Chunk* gameOver;
TTF_Font* font;

std::vector<Object*> box;
std::vector<Item*> coin;
std::vector<Monster*> enemy;
std::vector<Decor*> decor_things;
Item* health_bar;
Item* key_level;
Item* sword;
std::vector<Item*> heal;
std::vector<Object*> portal_gate;
std::vector<Button*> button;
std::vector<Effect*> effect;
std::vector<Effect*> enemy_effect;
Object* ball;
Object* ship;

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
    { {22, 12} },
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

std::vector<std::pair<int, int>> pos_tree = {
    {140, 1340},
    {14 * 80 - 23, 18 * 80},
    {17 * 80 - 23, 17 * 80}
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

    TTF_Init();
    font = TTF_OpenFont("assets/fonts/Stacked pixel.ttf", 30);

    // create player
    player = new Sprites(new Properties("player-idle", 28 * 80, 4 * 80, 64, 40, 5));
    
    // no sword    
    player->Load("player-idle", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose without Sword/01-Idle/Idle", 5);
    player->Load("player-run", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose without Sword/02-Run/Run", 6);
    player->Load("player-jump", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose without Sword/03-Jump/Jump", 3);
    player->Load("player-dead", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose without Sword/07-Dead Hit/Dead Hit", 4);
    player->Load("player-fall", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose without Sword/04-Fall/Fall", 1);

    // sword
    player->Load("player-idle-sw", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/09-Idle Sword/Idle Sword", 5);
    player->Load("player-run-sw", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/10-Run Sword/Run Sword", 6);
    player->Load("player-jump-sw", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/11-Jump Sword/Jump Sword", 3);
    player->Load("player-dead-sw", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/07-Dead Hit/Dead Hit Sword", 4);
    player->Load("player-ground-sw", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/05-Ground/Ground Sword", 2);
    player->Load("player-fall-sw", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/12-Fall Sword/Fall Sword", 1);
    player->Load("player-attack1", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/15-Attack 1/Attack1", 3);
    player->Load("player-attacked", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/14-Hit Sword/Hit Sword", 4);
    player->Load("player-attack2", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/16-Attack 2/Attack2", 3);
    player->Load("player-attack3", "assets/player/Sprites/Captain Clown Nose/Captain Clown Nose with Sword/17-Attack 3/Attack3", 3);
    
    // attack_effected;
    Effect* player_attack_effected1 = new Effect(new Properties("player-attack-effected1", 600, 600, 64, 40, 1.5));
    player_attack_effected1->Load("player-attack-effected1", "assets/player/Sword Effects/24-Attack 1/Attack effec1", 3);
    effect.push_back(player_attack_effected1);

    Effect* player_attack_effected2 = new Effect(new Properties("player-attack-effected2", 600, 600, 64, 40, 3));
    player_attack_effected2->Load("player-attack-effected2", "assets/player/Sword Effects/25-Attack 2/Attack 2", 3);
    effect.push_back(player_attack_effected2);

    Effect* player_attack_effected3 = new Effect(new Properties("player-attack-effected3", 600, 600, 64, 40, 3));
    player_attack_effected3->Load("player-attack-effected3", "assets/player/Sword Effects/26-Attack 3/Attack 3", 3);
    effect.push_back(player_attack_effected3);

    // heal
    health_bar = new Item(new Properties("health-bar", 17 * 80, 10 * 80, 154, 62, 3));
    health_bar->Load("health-bar", "assets/ui/Health_Bar", 1);
    health_bar->setType(3);

    // sword
    sword = new Item(new Properties("sword", 22 * 80, 16 * 80, 20, 20, 5));
    sword->Load("sword", "assets/player/Sprites/Sword/21-Sword Idle/Sword Idle", 8);
    sword->setType(5);

    for(int i = 0; i < NUM_OF_HEAL; i++) {
        Item* new_heal = new Item(new Properties("heal-full", 17 * 80 + i * 80, 10 * 80, 22, 19, 3));
        new_heal->Load("heal-full", "assets/ui/Heart", 1);
        new_heal->setType(1);
        heal.push_back(new_heal);
    }

    key_level = new Item(new Properties("key-level", 44 * 80, 14 * 80, 32, 32, 3));
    key_level->Load("key-level", "assets/Objects/key/key", 10);
    key_level->setType(4);

    // origin item
    player->addSword(sword);
    player->addEffect(effect);
    player->addHeal(heal);
    player->addHealthBar(health_bar);
    player->addFont(font);

    // map create
    MapParser::GetInstance()->Load();
    m_LevelMap = MapParser::GetInstance()->GetMap("GAME_MAP");

    Mouse::getInstance()->Load("mouse", "assets/inputs/slick_arrow-arrow.png");

    Button* play_button = new Button(new Properties("button-play", 1400, 1100, 96, 32, 3));
    play_button->setType(0);
    play_button->Load("button-play", "/home/thienq/Advanture Time/assets/ui/Buttons With Text/Text_Play_Button", 3);
    button.push_back(play_button);

    Button* setting_button = new Button(new Properties("button-setting", 1400, 1250, 96, 32, 3));
    setting_button->setType(1);
    setting_button->Load("button-setting", "assets/ui/Buttons With Text/Text_Settings_Button", 3);
    button.push_back(setting_button);

    Button* menu_button = new Button(new Properties("button-menu", 1400, 1400, 96, 32, 3));
    menu_button->setType(2);
    menu_button->Load("button-menu", "assets/ui/Buttons With Text/Text_Menu_Button", 3);
    button.push_back(menu_button);

    Button* exit_button = new Button(new Properties("button-exit", 1400, 1550, 96, 32, 3));
    exit_button->setType(3);
    exit_button->Load("button-exit", "assets/ui/Buttons With Text/Text_Exit_Button", 3);
    button.push_back(exit_button);

    TextureManager::GetInstance()->Load("menu-screen", "assets/ui/menu_screen.png");
    TextureManager::GetInstance()->Load("title", "assets/ui/title.png");

    SDL_Surface* surface = IMG_Load("assets/ui/menu_screen.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);

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
        Monster* new_enemy = new Monster(new Properties("monster", pos.first * 80, pos.second * 80, 72, 32, 5));
        new_enemy->Load("enemy-idle", "assets/enemy/Sprites/Crabby/01-Idle/Idle", 9);
        new_enemy->Load("enemy-run", "assets/enemy/Sprites/Crabby/02-Run/Run", 6);
        new_enemy->Load("enemy-attack", "assets/enemy/Sprites/Crabby/07-Attack/Attack", 4);
        new_enemy->setType(0);
        new_enemy->Load("enemy-hit", "assets/enemy/Sprites/Crabby/08-Hit/Hit", 4);
        new_enemy->Load("enemy-dead", "assets/enemy/Sprites/Crabby/09-Dead Hit/Dead Hit", 4);
        new_enemy->Load("enemy-deaded", "assets/enemy/Sprites/Crabby/10-Dead Ground/Dead Ground", 4);
        enemy.push_back(new_enemy);
    }
    Effect* crab_attack_effected = new Effect(new Properties("crab-attack-effected", 600, 600, 118, 24, 5));
    crab_attack_effected->Load("crab-attack-effected", "assets/enemy/Sprites/Crabby/11-Attack Effect/Attack Effect", 3);
    enemy_effect.push_back(crab_attack_effected);
    for(auto t : enemy) {
        t->addEffect(enemy_effect);
    }

    ball = new Object(new Properties("ball-idle", 10 * 80, 6 * 80, 16, 16, 5));
    ball->Load("ball-idle", "assets/trap/Cannon/Cannon Ball Idle/ball", 1);
    ball->Load("ball-destroy", "assets/trap/Cannon/Cannon Ball Destroyed/ball-destroy", 3);
    Object* exploring = new Object(new Properties("exploring", 10 * 80, 14 * 80, 54, 60, 5));
    exploring->Load("exploring", "assets/trap/Cannon/Cannon Ball Explosion/explore", 7);
    exploring->setType(3);
    ball->addExploration(exploring);
    ball->setType(2);

    ship = new Object(new Properties("ship-idle", 49 * 80, 18 * 80, 80, 26, 5));
    ship->Load("ship-idle", "assets/Objects/Sprites/Ship/Ship/Idle/ship", 6);
    ship->setType(4);

    Decor* platform = new Decor(new Properties("platform", 650, 1200, 30, 16, 5));
    platform->Load("platform", "assets/Decor/Flag/Platform", 1);
    platform->setType(1);
    decor_things.push_back(platform);

    for(auto pos : pos_tree) {
        Decor* tree = new Decor(new Properties("tree", pos.first, pos.second, 39, 32, 3.5));
        tree->Load("tree", "assets/Decor/Front Palm Trees/tree", 4);
        tree->setType(2);
        decor_things.push_back(tree);
    }

    Decor* flag = new Decor(new Properties("Flag-idle", 9 * 80, 980, 34, 93, 5));
    flag->Load("Flag-idle", "assets/Decor/Flag/Flag", 9);
    flag->setType(0);
    decor_things.push_back(flag);

    Decor* helm = new Decor(new Properties("helm-idle", 32 * 80, 13 * 80, 31, 32, 5));
    helm->Load("helm-idle", "assets/Decor/Ship Helm/idle", 6);
    helm->Load("helm-turn", "assets/Decor/Ship Helm/turn", 4);
    helm->setType(3);
    decor_things.push_back(helm);

    // add item
    player->addBox(box);
    player->addItem(coin);
    player->addEnemy(enemy);
    player->addGateLevel(portal_gate[0]);
    player->addBomb(ball);
    player->addShip(ship);

    // camera 
    Camera::GetInstance()->SetTarget(player->GetOrigin());
}

void Engine::Update() {
    double dt = Timer::getInstance()->getDeltaTime();
    Mouse::getInstance()->Update();
    if(!m_starting) {
        for(auto t : button) {
            t->Update(dt);
        }
    }
    if(m_starting) {
        for(int LEVEL = 1; LEVEL <= 3; LEVEL++) {
            if(player->getLevel() == LEVEL && !loaded_level[LEVEL - 1]) {
                Init_Level(LEVEL);
                loaded_level[LEVEL - 1] = true;
                cur_level = LEVEL;
            }
        }
        player->Update(dt);
        if(!ball->isToBeDestroyed()) {
            ball->Update(dt);
            ball->getExploration()->Update(dt);
        }
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
        if(!sword->isToBeDestroyed()) {
            sword->Update(dt);
        }
        key_level->Update(dt);
        ship->Update(dt);
        for(auto t : enemy) {
            t->Update(dt);
        }

        for(auto t : box) {
            t->Update(dt);
        }
        for(auto t : coin) {
            t->Update(dt);
        }
        health_bar->Update(dt);
        for(auto t : heal) {
            t->Update(dt);
        }
        for(auto t : portal_gate) {
            t->Update(dt);
        }
        for(auto t : decor_things) {
            t->Update(dt);
        }
        Camera::GetInstance()->Update(dt);
    }
}

bool Engine::Clean() {
    TextureManager::GetInstance()->Clean();
    SDL_DestroyWindow(m_Window);
    SDL_DestroyRenderer(m_Renderer);
    MapParser::GetInstance()->Clean();
    IMG_Quit();
    Mix_FreeChunk(gameMusic);
    Mix_FreeChunk(gameOver);
    Mix_Quit();Mouse::getInstance()->Draw();
    
    SDL_Quit();
    TTF_CloseFont(font);
    SDL_Log("Success Clean !!!\n");
}

void Engine::Render() {
    if(m_starting) {
        m_LevelMap->Render();
        player->Draw();
        key_level->Draw();
        ship->Draw();
        for(auto t : enemy) {
            t->Draw();
        }
        for(auto t : effect) {
            t->Draw();
        }
        for(auto t : enemy_effect) {
            t->Draw();
        }
        for(auto t : box) {
            t->Draw();
        }
        for(auto t : coin) {
            t->Draw();
        }
        health_bar->Draw();
        for(auto t : heal) {
            t->Draw();
        }
        for(auto t : portal_gate) {
            t->Draw();
        }
        for(auto t : decor_things) {
            t->Draw();
        }

        if(!sword->isToBeDestroyed()) {
            sword->Draw();
        }
        if(!ball->isToBeDestroyed()) {
            ball->Draw();
            ball->getExploration()->Draw();
        }
    }
    else {
        TextureManager::GetInstance()->DrawBackground("menu-screen", {0, 0, 2960, 1790});
        TextureManager::GetInstance()->DrawBackground("title", {1000, 50, 1024, 1024});
        for(auto t : button) {
            t->Draw();
        }
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