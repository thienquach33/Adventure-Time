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
std::vector<Item*> coin, bottle;
std::vector<Monster*> enemy;
std::vector<Decor*> decor_things;
Item* health_bar;
Item* key_level;
Item* sword;
std::vector<Item*> heal;
std::vector<Object*> portal_gate;
std::vector<Button*> button, inGame_button;
std::vector<Effect*> effect;
std::vector<Effect*> enemy_effect;
Object* ball;
Object* ship;

const int NUM_OF_HEAL = 3;

std::vector<std::pair<int, int>> postision_box[3] = {
     { {6, 15}, {15, 15} , {99, 16}, {126, 8}},
     {  },
     { }
};
std::vector<std::pair<int, int>> sliver_postision_item[3] = {
    { {12, 19}, {20, 16}, {27, 15}, {70, 16}, {103, 14}, {91, 19}, {124, 19}, {23, 5}, {17, 6}, {97, 17}},
    { },
    { }
};
std::vector<std::pair<int, int>> enemy_postision[3] = {
    { {22, 12}, {45, 14}, {73, 14}}
};

std::vector<std::pair<int, int>> pink_start_pos = {
    {8, 6}
};
std::vector<std::pair<int, int>> gold_postision_item[3] = {
    { {2, 16} , {12, 7}, {33, 14}, {46, 15}, {126, 9}, {74, 14}, {83, 19}, {29, 6}},
    { },
    { }
};
std::vector<std::pair<int, int>> portal_gate_postision[3] = {
    { {45, 13} },
    {  },
    {  } 
};

std::vector<std::pair<int, int>> pos_tree = {
    {140, 1340},
    {14 * 80 - 23, 18 * 80},
    {17 * 80 - 23, 17 * 80}
};

int gameMusicChannel = -1;

bool Engine::Init() {
    // init SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) return false;
    m_Window = SDL_CreateWindow("Advanture Time", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Log("Success Init !!!\n");
    

    // init Mix
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 128);
    gameMusic = Mix_LoadWAV("assets/sounds/music_game.wav");
    gameMusicChannel = Mix_PlayChannel(-1, gameMusic, -1);
    gameOver = Mix_LoadWAV("assets/sounds/round_end.wav");
    Mix_Volume(-1, MIX_MAX_VOLUME / 2); // 50% volume


    TTF_Init();
    font = TTF_OpenFont("assets/fonts/Stacked pixel.ttf", 30);

    // create player
    player = new Sprites(new Properties("player-idle", 8 * 80, 15 * 80, 64, 40, 5));
    
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
    player->Load("menu", "assets/ui/menu", 1);

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
    play_button->Load("button-play", "assets/ui/Buttons With Text/Text_Play_Button", 3);
    button.push_back(play_button);

    Button* setting_button = new Button(new Properties("button-setting", 1400, 1250, 96, 32, 3));
    setting_button->setType(1);
    setting_button->Load("button-setting", "assets/ui/Buttons With Text/Text_Settings_Button", 3);
    button.push_back(setting_button);

    Button* menu_button = new Button(new Properties("button-menu", 1400, 1400, 96, 32, 3));
    menu_button->setType(2);
    menu_button->Load("button-menu", "assets/ui/Buttons With Text/Text_Menu_Button", 3);
    button.push_back(menu_button);

    Button* continue_button = new Button(new Properties("button-continue", 1400, 1100, 96, 32, 3));
    continue_button->setType(5);
    continue_button->Load("button-continue", "assets/ui/Buttons With Text/Text_Continue_Button", 3);

    Button* exit_button = new Button(new Properties("button-exit", 1400, 1550, 96, 32, 3));
    exit_button->setType(3);
    exit_button->Load("button-exit", "assets/ui/Buttons With Text/Text_Exit_Button", 3);
    button.push_back(exit_button);

    // Button* pause_button = new Button(new Properties("button-pause", 1400, 1400, 96, 32, 3));
    // pause_button->setType(4);
    // pause_button->Load("button-pause", "assets/ui/button/pause/342 px/pause", 3);
    // inGame_button.push_back(pause_button);

    TextureManager::GetInstance()->Load("menu-screen", "assets/ui/menu_screen.png");
    TextureManager::GetInstance()->Load("title", "assets/ui/title.png");
    TextureManager::GetInstance()->Load("menu-game", "assets/ui/menu-0.png");

    for(int i = 0; i < 12; i++) {
        std::string game_over_banner = "banner-" + std::to_string(i);
        std::string name_path = "assets/ui/Yellow Board/banner-" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(game_over_banner, name_path);
    }

    for(int i = 0; i < 12; i++) {
        std::string game_over_banner = "paper-" + std::to_string(i);
        std::string name_path = "assets/ui/Orange Paper/paper-" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(game_over_banner, name_path);    
    }

    for(int i = 1; i <= 52; i++) {
        std::string name_text = "text-" + std::to_string(i);
        std::string name_path = "assets/ui/Small Text/" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(name_text, name_path);
    }
    for(int i = 1; i <= 36; i++) {
        std::string name_text = "big-text-" + std::to_string(i);
        std::string name_path = "assets/ui/Big Text/" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(name_text, name_path);
    }

    for(int i = 0; i < 13; i++) {
        std::string game_over_banner = "banner-p-" + std::to_string(i);
        std::string name_path = "assets/ui/Small Banner/banner-p-" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(game_over_banner, name_path);    
    }

    for(int i = 0; i < 26; i++) {
        std::string button_name = "icon-" + std::to_string(i);
        std::string name_path = "assets/ui/Small Icons/" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(button_name, name_path);
    }

    for(int i = 0; i < 17; i++) {
        std::string button_name = "yellow-button-" + std::to_string(i);
        std::string name_path = "assets/ui/Yellow Button/" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(button_name, name_path);
    }

    for(int i = 1; i < 17; i++) {
        std::string green_board = "green-board-" + std::to_string(i);
        std::string name_path = "assets/ui/Green Board/" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(green_board, name_path);
    }

    for(int i = 1; i < 17; i++) {
        std::string yellow_paper = "yellow-paper-" + std::to_string(i);
        std::string name_path = "assets/ui/Yellow Paper/" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(yellow_paper, name_path);
    }

    for(int i = 1; i < 17; i++) {
        std::string green_button = "green-button-" + std::to_string(i);
        std::string name_path = "assets/ui/Green Button/" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(green_button, name_path);   
    }

    for(int i = 1; i < 13; i++) {
        std::string text_name = "prefabs-" + std::to_string(i);
        std::string name_path = "assets/ui/Prefabs/" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(text_name, name_path);
    }

    for(int i = 1; i <= 10; i++) {
        std::string slider_name = "slider-" + std::to_string(i);
        std::string name_path = "assets/ui/Sliders/" + std::to_string(i) + ".png";
        TextureManager::GetInstance()->Load(slider_name, name_path);
    }

    SDL_Surface* surface = IMG_Load("assets/ui/menu_screen.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);

    return m_isRunning = true;
}

std::vector<std::pair<int, int>> banner_p_highcore = {
    {1160, 300}, {1380, 300}, {1600, 300}, {1800, 300}
};

std::vector<std::string> banner_p_highcore_text = {
    "0", "12", "12", "1"
};

void Engine::highScore() {
    if(high_score_screen == false) return;
    int bonus = Camera::GetInstance()->GetPostision().X;
    for(int i = 0; i < 4; i++) {
        std::string banner_p_highcore_name = "banner-p-" + banner_p_highcore_text[i];
        TextureManager::GetInstance()->DrawMenu(banner_p_highcore_name, banner_p_highcore[i].first + bonus - 50, banner_p_highcore[i].second, 32, 32, 7);
    }
    std::string leader_board = "leaderboard";
    for(int i = 0; i < (int) leader_board.size(); i++) {
        std::string text_name = "big-text-" + std::to_string(leader_board[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1280 + i * 48 + bonus, 380, 10, 11, 4);
    }
    TextureManager::GetInstance()->DrawMenu("prefabs-3", 1220 + bonus, 500, 128, 128, 5);

    int cnt = 0;

    for(auto s : HighScore) {
        if(cnt >= 5) break;
        std::string text_name = "rank " + std::to_string(++cnt) + "   " + std::to_string(s);
        for(int i = 0; i < (int) text_name.size(); i++) {
            std::string temp;
            if(text_name[i] >= '1' && text_name[i] <= '9') {
                temp = "big-text-" + std::to_string(text_name[i] - '0' + 26);
            }
            if(text_name[i] == '0') temp = "big-text-36";
            if(text_name[i] >= 'a' && text_name[i] <= 'z') {
                temp = "big-text-" + std::to_string(text_name[i] - 'a' + 1);
            }
            // if(text_name[i] == '#') temp = "big-text-44";
            // if(text_name[i] == '.') temp = "big-text-48";
            TextureManager::GetInstance()->DrawMenu(temp, 1300 + i * 40 + bonus, 520 + cnt * 100, 10, 11, 4);
        }
    }
}

const int NUM_OF_MAIN_MENU_GREEN_BOARD = 22;

std::vector<std::pair<int, int>> banner_green_board = {
    {1160, 500}, {1320, 500}, {1480, 500}, {1640, 500}, {1800, 500},
    {1160, 660}, {1320, 660}, {1480, 660}, {1640, 660}, {1800, 660},
    {1160, 820}, {1320, 820}, {1480, 820}, {1640, 820}, {1800, 820},
    {1160, 930}, {1320, 930}, {1480, 930}, {1640, 930}, {1800, 930},
    {2000, 500}, {2000, 660}
};

std::vector<std::string> green_board = {
    "1", "2", "2", "2", "3",
    "4", "5", "5", "5", "6",
    "4", "5", "5", "5", "6",
    "7", "8", "8", "8", "9",
    "13", "15"
};

const int NUM_OF_YELLOW_PAPER = 11;

std::vector<std::pair<int, int>> yellow_paper_pos = {
    {1160, 520}, {1320, 520}, {1480, 520}, {1640, 520}, {1800, 520},
    {1160, 680}, {1320, 680}, {1480, 680}, {1640, 680}, {1800, 680}, 
    {1997, 530}
};

std::vector<std::string> yellow_paper_text = {
    "1", "2", "2", "2", "3",
    "7", "8", "8", "8", "9", 
    "16"
};

const int NUM_OF_GREEN_BUTTON = 14;
std::vector<std::pair<int, int>> green_button = {
    {1250, 800}, {1250, 900}, {1700, 800}, {1996, 670}, {1110, 500},
    {1420, 780}, {1490, 780}, {1560, 780},
    {1420, 850}, {1490, 850}, {1560, 850},
    {1420, 920}, {1490, 920}, {1560, 920}
};

std::vector<std::string> green_button_text = {
    "1", "1", "1", "1", "1",
    "8", "9", "10", "11", "12", "13", "14", "15", "16"
};

const int NUM_OF_BANNER_P_MENU = 3;

std::vector<std::pair<int, int>> banner_p_menu_pos = {
    {1750, 870}, {1900, 870}, {2050, 895}
};

std::vector<std::string> banner_p_menu_text = {
    "10", "4", "1"
};

std::vector<std::pair<int, int>> icon_menu_pos = {
    {1250, 800}, {1250, 900}, {1700, 800}, {1996, 670}, {1110, 500}, {1997, 580}
};

std::vector<std::string> icon_menu = {
    "20", "12", "16", "7", "1", "17"
};

void Engine::menuGame() {
    if(!menu_screen) return;
    int bonus = Camera::GetInstance()->GetPostision().X;
    for(int i = 0; i < NUM_OF_MAIN_MENU_GREEN_BOARD; i++) {
        std::string green_board_name = "green-board-" + green_board[i];
        TextureManager::GetInstance()->DrawMenu(green_board_name, banner_green_board[i].first - 100 + bonus, banner_green_board[i].second, 32, 32, 5);
    }
    for(int i = 0; i < NUM_OF_YELLOW_PAPER; i++) {
        std::string yellow_paper_name = "yellow-paper-" + yellow_paper_text[i];
        TextureManager::GetInstance()->DrawMenu(yellow_paper_name, yellow_paper_pos[i].first - 100 + bonus, yellow_paper_pos[i].second, 32, 32, 5);
    }
    std::string main_menu_text = "main menu";

    for(int i = 0; i < (int) main_menu_text.size(); i++) {
        std::string text_name = "text-" + std::to_string(main_menu_text[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1200 + i * 54 + bonus, 650, 5, 6, 9);
    }

    for(int i = 0; i < NUM_OF_GREEN_BUTTON; i++) {
        std::string green_button_name = "green-button-" + green_button_text[i];
        TextureManager::GetInstance()->DrawMenu(green_button_name, green_button[i].first - 50 + bonus, green_button[i].second, 14, 14, 5);

        SDL_Rect temp = {green_button[i].first - 50, green_button[i].second, 14 * 5, 14 * 5};
        SDL_Rect temp_mouse = Mouse::getInstance()->getPoint();
        if(SDL_HasIntersection(&temp, &temp_mouse)) {
            if(Mouse::getInstance()->oneClickedCheck()) {
                if(i == 4) {
                    menu_screen = false;
                    return;
                }
                else if(i == 0) {
                    score_game = 0;
                    menu_screen = false;
                    Init_Level(1);
                    player->setPos();
                    return;
                }
                else if(i == 1) {
                    menu_screen = false;
                    setting_screen = true;
                    return;
                }
                else if(i == 2) {
                    high_score_screen = true;
                    menu_screen = false;
                    return;
                }
                else if(i >= 5 && i <= 13) {
                    menu_screen = false;
                    return;
                }
                else if(i == 3) {
                    system("xdg-open https://www.facebook.com/duck.3305/");
                }
            }
        }
    }

    std::string play_text = "play";

    for(int i = 0; i < (int) play_text.size(); i++) {
        std::string text_name = "text-" + std::to_string(play_text[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1385 + i * 48 + bonus, 855, 5, 6, 8);
    }

    for(int i = 0; i < NUM_OF_BANNER_P_MENU; i++) {
        std::string banner_p_menu_name = "banner-p-" + banner_p_menu_text[i];
        TextureManager::GetInstance()->DrawMenu(banner_p_menu_name, banner_p_menu_pos[i].first - 100 + bonus, banner_p_menu_pos[i].second, 32, 32, 5);

        if(i == 0 || i == 2) {

            SDL_Rect temp = {banner_p_menu_pos[i].first - ((i == 0) ? 15 : 185), banner_p_menu_pos[i].second + 50, 32 * 4, 60};
            SDL_Rect temp_mouse = Mouse::getInstance()->getPoint();

            if(SDL_HasIntersection(&temp, &temp_mouse)) {
                if(Mouse::getInstance()->oneClickedCheck()) {
                    if(i == 2) {
                        m_starting = false;
                        continue_screen = true;
                        menu_screen = false;
                        return;
                    }
                    else {
                        system("xdg-open https://github.com/thienquach33");
                    }
                }
            }
        }
    }

    std::string info_text = "info";
    for(int i = 0; i < (int) info_text.size(); i++) {
        std::string text_name = "text-" + std::to_string(info_text[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1745 + i * 30 + bonus, 930, 5, 6, 5);
    }
    std::string exit_text = "exit";
    for(int i = 0; i < (int) exit_text.size(); i++) {
        std::string text_name = "text-" + std::to_string(exit_text[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1880 + i * 30 + bonus, 950, 5, 6, 5);
    }
    for(int i = 0; i < 6; i++) {
        std::string icon_name = "icon-" + icon_menu[i];
        TextureManager::GetInstance()->DrawMenu(icon_name, icon_menu_pos[i].first - 35 + bonus, icon_menu_pos[i].second + 15, 8, 6, 5);
    }
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


    bottle.clear();
    Item* new_green_bottle = new Item(new Properties("green-bottle", 46 * 80, 9 * 80 - 20, 13, 17, 4));
    new_green_bottle->Load("green-bottle", "assets/item/Sprites/Green Bottle/green-bottle", 7);
    new_green_bottle->setType(6);
    bottle.push_back(new_green_bottle);

    portal_gate.clear();
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
    for(auto t : enemy) {
        enemy_effect.clear();
        Effect* crab_attack_effected = new Effect(new Properties("crab-attack-effected", 600, 600, 118, 24, 5));
        crab_attack_effected->Load("crab-attack-effected", "assets/enemy/Sprites/Crabby/11-Attack Effect/Attack Effect", 3);
        enemy_effect.push_back(crab_attack_effected);
        t->addEffect(enemy_effect);
    }

    for(auto pos : pink_start_pos) {
        Monster* new_enemy = new Monster(new Properties("pink-star", pos.first * 80, pos.second * 80, 34, 30, 5));
        new_enemy->Load("pink-star-idle", "assets/enemy/Sprites/Pink Star/01-Idle/Idle", 8);
        new_enemy->Load("pink-star-attack", "assets/enemy/Sprites/Pink Star/07-Attack/Attack", 4);
        new_enemy->setType(1);
        enemy.push_back(new_enemy);
    }

    Monster* enemy_totem = new Monster(new Properties("totem", 78 * 80, 15 * 80, 60, 32, 4));
    enemy_totem->Load("totem-idle", "assets/enemy/Sprites/Totems/Head 1/Idle 1/idle", 1);
    enemy_totem->Load("totem-attack", "assets/enemy/Sprites/Totems/Head 1/Attack 1/attack", 6);
    enemy_totem->setType(2);
    enemy.push_back(enemy_totem);

    ball = new Object(new Properties("ball-idle", 29 * 80, -3 * 80, 16, 16, 5));
    ball->Load("ball-idle", "assets/trap/Cannon/Cannon Ball Idle/ball", 1);
    ball->Load("ball-destroy", "assets/trap/Cannon/Cannon Ball Destroyed/ball-destroy", 3);
    Object* exploring = new Object(new Properties("exploring", 28 * 80, 5 * 80, 54, 60, 5));
    exploring->Load("exploring", "assets/trap/Cannon/Cannon Ball Explosion/explore", 7);
    exploring->setType(3);
    ball->addExploration(exploring);
    ball->setType(2);

    ship = new Object(new Properties("ship-idle", 49 * 80, 18 * 80, 80, 26, 5));
    ship->Load("ship-idle", "assets/Objects/Sprites/Ship/Ship/Idle/ship", 6);
    ship->setType(4);

    decor_things.clear();
    Decor* sail = new Decor(new Properties("sail-no-wind", 4120, 1200, 28, 50, 5));
    sail->Load("sail-no-wind", "assets/Decor/Sail/No Wind/sail-no-wind", 8);
    sail->Load("sail-wind", "assets/Decor/Sail/Wind/sail-wind", 4);
    sail->Load("sail-tf-no-wind", "assets/Decor/Sail/Transition to No Wind/sail-tf-no", 11);
    sail->Load("sail-tf-wind", "assets/Decor/Sail/Transition to Wind/sail-tf-wind", 3);
    sail->setType(4);
    decor_things.push_back(sail);

    ship->addSail(sail);

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

    sword = new Item(new Properties("sword", 22 * 80, 16 * 80, 20, 20, 5));
    sword->Load("sword", "assets/player/Sprites/Sword/21-Sword Idle/Sword Idle", 8);
    sword->setType(5);

    // add item
    player->addSword(sword);
    player->addBottle(bottle);
    player->addBox(box);
    player->addItem(coin);
    player->addEnemy(enemy);
    player->addGateLevel(portal_gate[0]);
    player->addBomb(ball);
    player->addShip(ship);

    // camera 
    Camera::GetInstance()->SetTarget(player->GetOrigin());
}

int cur_volume = 2;

void Engine::Update() {
    if(music == false) {
        Mix_Pause(gameMusicChannel);
    }
    else {
        Mix_Resume(gameMusicChannel);
    }
    Mix_Volume(-1, MIX_MAX_VOLUME * cur_volume * 0.25);
    double dt = Timer::getInstance()->getDeltaTime();
    Mouse::getInstance()->Update();
    if(!m_starting) {
        for(auto t : button) {
            t->Update(dt);
        }
    }
    if(m_starting && !game_over_screen && !menu_screen && !high_score_screen && !setting_screen) {
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
        for(auto it = bottle.begin(); it != bottle.end(); /* no increment here */) {
            if((*it)->isToBeDestroyed()) {
                it = bottle.erase(it);
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
        for(auto t : bottle) {
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

const int NUM_OF_BANNER = 22;
std::vector<std::pair<int, int>> banner = { 
    {1000, 500} , {1160, 500} , {1320, 500} , {1480, 500} , {1640, 500} , {1800, 500},
    {1000, 660}, {1160, 660}, {1320, 660}, {1480, 660}, {1640, 660}, {1800, 660},
    {1000, 820}, {1160, 820}, {1320, 820}, {1480, 820}, {1640, 820}, {1800, 820},
    {1160, 1000}, {1320, 1000}, {1480, 1000}, {1640, 1000}
};
std::vector<std::string> banner_text = { 
    "0", "1" , "1", "1", "1", "2",
    "3", "4", "4", "4", "4", "5",
    "6", "7", "7", "7", "7", "8",
    "9", "10", "10", "11"
};

const int NUM_OF_PAPER = 18;
std::vector<std::pair<int, int>> paper = { 
    {1005, 500}, {1165, 500}, {1325, 500}, {1485, 500}, {1645, 500}, {1805, 500},
    {1005, 660}, {1165, 660}, {1325, 660}, {1485, 660}, {1645, 660}, {1805, 660},
    {1005, 820}, {1165, 820}, {1325, 820}, {1485, 820}, {1645, 820}, {1805, 820},
};

std::vector<std::string> paper_text = { 
    "0", "1", "1", "1", "1", "2",
    "3", "4", "4", "4", "4", "5",
    "6", "7", "7", "7", "7", "8",
};

const int NUM_OF_BANNER_P = 5;
std::vector<std::pair<int, int>> banner_p = {
    {1000, 300}, {1200, 300}, {1420 - 60 - 30, 260}, {1620 - 60 - 30, 260}, {1820 - 130, 260}
};
std::vector<std::string> banner_text_p = {
    "0", "12", "5", "12", "1"
};
std::vector<std::pair<int, int>> exit_banner = {
    {1800 - 230, 990}, {1990 - 230, 990}
};
std::vector<std::string> exit_banner_t = {
    "10", "1"
};

std::vector<std::string> yellow_button_text = {
    "1", "2", "4"
};

std::vector<std::pair<int, int>> yellow_button = {
    {1250, 1043}, {1400, 1043}, {1470, 1043}
};

std::vector<std::string> icon_td = {
    "16", "9"
};

std::vector<std::pair<int, int>> icon_pos = {
    {1265, 1058}, {1450, 1058}
};

void Engine::GameOverScreen() {
    if(game_over_screen == false || high_score_screen) return;
    int bonus = Camera::GetInstance()->GetPostision().X;
    for(int i = 0; i < NUM_OF_BANNER; i++) {
        std::string game_over_banner = "banner-" + banner_text[i];
        TextureManager::GetInstance()->DrawMenu(game_over_banner, banner[i].first + bonus, banner[i].second, 32, 32, 5);
    }

    for(int i = 0; i < NUM_OF_PAPER; i++) {
        std::string game_over_paper = "paper-" + paper_text[i];
        TextureManager::GetInstance()->DrawMenu(game_over_paper, paper[i].first - 3 + bonus, paper[i].second, 32, 32, 5);    
    }

    std::string score = "score:" + std::to_string(score_game);
    for(int i = 0; i < (int) score.size(); i++) {
        std::string text_name = "text-" + std::to_string(score[i] - 'a' + 1);
        if(score[i] == ':') text_name = "text-50";
        if(score[i] >= '1' && score[i] <= '9') {
            text_name = "text-" + std::to_string(score[i] - '0' + 26);
        }
        if(score[i] == '0') text_name = "text-36";
        TextureManager::GetInstance()->DrawMenu(text_name, 1200 + i * 60 + bonus, 700, 5, 6, 10);
    }

    for(int i = 0; i < NUM_OF_BANNER_P; i++) {
        std::string game_over_paper = "banner-p-" + banner_text_p[i];
        TextureManager::GetInstance()->DrawMenu(game_over_paper, banner_p[i].first - 3 - 30 + 15 + bonus, banner_p[i].second, 32, 32, 8);    
    }

    std::string game_text = "game";
    for(int i = 0; i < (int) game_text.size(); i++) {
        std::string text_name = "big-text-" + std::to_string(game_text[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1250 + i * 50 - 70 + 12 + bonus, 395, 10, 11, 5);
    }
    std::string over_text = "over";
    for(int i = 0; i < (int) over_text.size(); i++) {
        std::string text_name = "big-text-" + std::to_string(over_text[i] - 'a' + 1);
        if(over_text[i] == '!') text_name = "46";
        TextureManager::GetInstance()->DrawMenu(text_name, 1630 + i * 50 - 120 + 12 + bonus, 355, 10, 11, 5);
    }
    for(int i = 0; i < 2; i++) {
        std::string exit_banner_name = "banner-p-" + exit_banner_t[i];
        TextureManager::GetInstance()->DrawMenu(exit_banner_name, exit_banner[i].first - 3 - 30 + 15 + bonus, exit_banner[i].second, 32, 32, 6);
    }
    std::string exit_text = "exit";
    for(int i = 0; i < (int) exit_text.size(); i++) {
        std::string exit_text_t = "text-" + std::to_string(exit_text[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(exit_text_t, 1910 - 180 + i * 36 - 70 + 12 + bonus, 1058, 5, 6, 6);

        SDL_Rect temp = {1910 - 180 + i * 36 - 70 + 12, 1058, 5 * 6, 6 * 5};
        SDL_Rect temp_mouse = Mouse::getInstance()->getPoint();
        if(SDL_HasIntersection(&temp, &temp_mouse)) {
            if(Mouse::getInstance()->oneClickedCheck()) {
                game_over_screen = false;
                m_starting = false;
                return;
            }
        }
    }

    for(int i = 0; i < 3; i++) {
        std::string yellow_button_name = "yellow-button-" + yellow_button_text[i];
        TextureManager::GetInstance()->DrawMenu(yellow_button_name, yellow_button[i].first + bonus, yellow_button[i].second, 14, 14, 5);                

        SDL_Rect temp = {yellow_button[i].first, yellow_button[i].second, 14 * 5, 14 * 5};
        SDL_Rect temp_mouse = Mouse::getInstance()->getPoint();
        if(SDL_HasIntersection(&temp, &temp_mouse)) {
            if(Mouse::getInstance()->oneClickedCheck()) {
                if(i == 0) {
                    high_score_screen = true;
                }
                else {
                    score_game = 0;
                    game_over_screen = false;
                    Init_Level(1);
                    return;
                }
            }
        }
    }

    for(int i = 0; i < 2; i++) {
        std::string icon_name = "icon-" + icon_td[i];
        TextureManager::GetInstance()->DrawMenu(icon_name, icon_pos[i].first + bonus, icon_pos[i].second, 8, 6, 5);
    }
}

const int NUM_OF_MAIN_MENU_GREEN_BOARD_SM = 20;

std::vector<std::pair<int, int>> banner_green_board_sm = {
    {1160, 500}, {1320, 500}, {1480, 500}, {1640, 500}, 
    {1160, 660}, {1320, 660}, {1480, 660}, {1640, 660},
    {1160, 820}, {1320, 820}, {1480, 820}, {1640, 820}, 
    {1160, 930}, {1320, 930}, {1480, 930}, {1640, 930}, 
    {1160, 1100}, {1320, 1100}, {1480, 1100}, {1640, 1100}
};

std::vector<std::string> green_board_sm = {
    "1", "2", "2", "3",
    "4", "5", "5", "6",
    "4", "5", "5", "6",
    "7", "8", "8", "9",
    "10", "11", "11", "12"
};

const int NUM_OF_YELLOW_PAPER_sm = 16;

std::vector<std::pair<int, int>> yellow_paper_pos_sm = {
    {1160, 520}, {1320, 520}, {1480, 520}, {1640, 520},
    {1160, 630}, {1320, 630}, {1480, 630}, {1640, 630},
    {1160, 730}, {1320, 730}, {1480, 730}, {1640, 730},
    {1160, 800}, {1320, 800}, {1480, 800}, {1640, 800}
};

std::vector<std::string> yellow_paper_text_sm = {
    "1", "2", "2", "3",
    "4", "5", "5", "6",
    "7", "8", "8", "9",
    "10", "11", "11", "12"
};

std::vector<std::string> yellow_button_sm = {
    "1", "1"
};

std::vector<std::pair<int, int>> yellow_button_pos_sm = {
    {1550, 610}, {1550, 720}
};

std::vector<std::pair<int, int>> icon_menu_pos_sm = {
    {1550, 610}, {1550, 720}
};

std::vector<int> yellow_button_state = {0, 0};

std::vector<std::string> slilder_text = {
   "1" ,"4", "5", "5", "5", "6", "2"
};

std::vector<std::pair<int, int>> slider_volume = {
    {1250, 950},{1300, 950}, {1340, 950}, {1390, 950}, {1430, 950}, {1470, 950}, {1520, 950}
};

std::vector<std::pair<int, int>> orange_paper_sm = {
    {1160, 1100}, {1320, 1100}, {1480, 1100}, {1640, 1100}
};

std::vector<std::string> orange_paper_text_sm = {
    "9", "10", "10", "11"
};

void Engine::settingMenu() {
    if(setting_screen == false) return;
    int bonus = Camera::GetInstance()->GetPostision().X;
    for(int i = 0; i < NUM_OF_MAIN_MENU_GREEN_BOARD_SM; i++) {
        std::string green_board_name = "green-board-" + green_board_sm[i];
        TextureManager::GetInstance()->DrawMenu(green_board_name, banner_green_board_sm[i].first + bonus + 50, banner_green_board_sm[i].second, 32, 32, 5);
    }
    for(int i = 0; i < NUM_OF_YELLOW_PAPER_sm; i++) {
        std::string yellow_paper_name = "yellow-paper-" + yellow_paper_text_sm[i];
        TextureManager::GetInstance()->DrawMenu(yellow_paper_name, yellow_paper_pos_sm[i].first + bonus + 50, yellow_paper_pos_sm[i].second, 32, 32, 5);
    }

    std::string music_text = "music";
    for(int i = 0; i < (int) music_text.size(); i++) {
        std::string text_name = "text-" + std::to_string(music_text[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1350 + i * 42 + bonus, 630, 5, 6, 7);
    }
    std::string sfx_text = "sfx";
    for(int i = 0; i < (int) sfx_text.size(); i++) {
        std::string text_name = "text-" + std::to_string(sfx_text[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1350 + i * 42 + bonus, 730, 5, 6, 7);
    }
    for(int i = 0; i < 2; i++) {
        std::string yellow_button_name = "yellow-button-" + yellow_button_sm[i];
        TextureManager::GetInstance()->DrawMenu(yellow_button_name, yellow_button_pos_sm[i].first + bonus + 100, yellow_button_pos_sm[i].second, 14, 14, 5.5);

        SDL_Rect temp = {yellow_button_pos_sm[i].first + 15 + 100, yellow_button_pos_sm[i].second, 14 * 5, 14 * 5};
        SDL_Rect temp_mouse = Mouse::getInstance()->getPoint();
        if(SDL_HasIntersection(&temp, &temp_mouse)) {
            if(Mouse::getInstance()->oneClickedCheck()) {
                yellow_button_state[i] ^= 1;
            }
        }
    }
    for(int i = 0; i < 2; i++) {
        std::string icon_name = "icon-";
        if(yellow_button_state[i] == 0) icon_name += "6";
        else icon_name += "5";
        if(yellow_button_state[i] == 0) {
            if(i == 0) music = true;
            else sfx = true;
        }
        else {
            if(i == 0) music = false;
            else sfx = false;
        }
        TextureManager::GetInstance()->DrawMenu(icon_name, icon_menu_pos_sm[i].first + 15 + bonus + 100, icon_menu_pos_sm[i].second + 15, 8, 6, 5.5);
    }

    std::string volume = "volume";
    for(int i = 0; i < (int) volume.size(); i++) {
        std::string text_name = "text-" + std::to_string(volume[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1400 + i * 42 + bonus, 860, 5, 6, 7);
    }

    for(int i = 0; i < 7; i++) {
        std::string slilder_name = "slider-" + slilder_text[i];
        TextureManager::GetInstance()->DrawMenu(slilder_name, slider_volume[i].first + bonus + 100, slider_volume[i].second, 12, 12, 5);

        if(i == 0 || i == 6) {

            SDL_Rect temp = {slider_volume[i].first + 100, slider_volume[i].second, 12 * 5, 12 * 5};
            SDL_Rect temp_mouse = Mouse::getInstance()->getPoint();
            if(SDL_HasIntersection(&temp, &temp_mouse)) {
                if(Mouse::getInstance()->oneClickedCheck()) {
                    cur_volume += (i == 0 ? -1 : 1);
                    cur_volume = std::max(0, cur_volume);
                    cur_volume = std::min(4, cur_volume);
                }
            }
        }
    }

    TextureManager::GetInstance()->DrawMenu("slider-3", 42.5 * cur_volume + bonus + 1400, 950, 12, 12, 5);

    for(int i = 0; i < 4; i++) {
        std::string orange_paper_name = "paper-" + orange_paper_text_sm[i];
        TextureManager::GetInstance()->DrawMenu(orange_paper_name, orange_paper_sm[i].first + bonus + 50, orange_paper_sm[i].second, 32, 32, 5);
    }

    std::string settings = "setting";
    for(int i = 0; i < (int) settings.size(); i++) {
        std::string text_name = "text-" + std::to_string(settings[i] - 'a' + 1);
        TextureManager::GetInstance()->DrawMenu(text_name, 1280 + i * 42 + bonus + 100, 1160, 5, 6, 7);
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
    Mix_Quit();
    
    SDL_Quit();
    TTF_CloseFont(font);
    SDL_Log("Success Clean !!!\n");
}

void Engine::Render() {
    SDL_RenderClear(m_Renderer);
    darker = (game_over_screen == true || menu_screen == true || high_score_screen == true || (setting_screen && m_starting));
    if(m_starting) {
        m_LevelMap->Render();
        player->Draw();
        key_level->Draw();
        ship->Draw();

        int bonus = Camera::GetInstance()->GetPostision().X;

        std::string current_score = "score " + std::to_string(score_game);

        for(int i = 0; i < (int) current_score.size(); i++) {
            std::string text_name = "big-text-" + std::to_string(current_score[i] - 'a' + 1);
            if(current_score[i] >= '1' && current_score[i] <= '9') {
                text_name = "big-text-" + std::to_string(current_score[i] - '0' + 26);
            }
            if(current_score[i] == '0') text_name = "big-text-36";
            TextureManager::GetInstance()->Draw(text_name, 500 + i * 60 + bonus, 100, 10, 11, 6);
        }

        for(auto t : enemy) {
            t->Draw();
            for(auto d : t->getEffect()) {
                d->Draw();
            }
        }
        for(auto t : effect) {
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
        for(auto t : bottle) {
            t->Draw();
        }

        if(!sword->isToBeDestroyed()) {
            sword->Draw();
        }
        if(!ball->isToBeDestroyed()) {
            ball->Draw();
            ball->getExploration()->Draw();
        }
        for(auto t : inGame_button) {
            t->Draw();
        }
    }
    else {
        TextureManager::GetInstance()->DrawBackground("menu-screen", {0, 0, 2960, 1790});
        if(!setting_screen) {
            TextureManager::GetInstance()->DrawBackground("title", {1000, 50, 1024, 1024});
            for(auto t : button) {
                t->Draw();
            }
        }
    }
    if(esc_menu) {
        TextureManager::GetInstance()->Draw("menu-game", 0, 0, 128, 128, 5);
    }
    if(player->getTurn() > 0) {
        GameOverScreen();
        // Quit();
    } 
    menuGame();
    highScore();
    settingMenu();
    Mouse::getInstance()->Draw();
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
    Input::getInstance()->Listen();
}

void Engine::Quit() {
    m_isRunning = false;
}