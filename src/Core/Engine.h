#ifndef ENGINE_H
#define ENGINE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <set>
#include "../map/GameMap.h"
#include <fstream>

#define SCREEN_WIDTH 2960
#define SCREEN_HEIGHT 1760

class Engine {
    public:
        static Engine* GetInstance() { return s_Instance = (s_Instance != nullptr ? s_Instance : new Engine()); }

        bool Init();
        bool Clean();
        void Quit();
        void menuScreen();

        void Init_Level(int level);

        void Update();
        void Render();
        void Events();
        void menuGame();
        void GameOverScreen();
        void GameWinner();
        void settingMenu();
        void highScore();

        GameMap* GetMap() { return m_LevelMap; }

        inline bool isRunning() { return m_isRunning; }
        inline bool isStarting() { return m_starting; }
        inline  SDL_Renderer* getRenderer() { return m_Renderer; }

        void setStarting(bool start) { m_starting = start; }
        void setRunning(bool run) { m_isRunning = run; }
        void SetGameOver(bool game_over) { game_over_screen = game_over; }
        void setSetting(bool setting) { setting_screen = setting; }
        void setMainMenu() { 
            if(!m_starting || game_over_screen || winner) return;
            menu_screen ^= 1; 
        }
        void setHighscore() {
            if(high_score_screen) {
                high_score_screen = false;
            }
        }
        bool getWinner() { return winner; }
        void setSetting() {
            if(setting_screen) {
                setting_screen = false;
            }
        }

        void InsertMap(int x) {
            MapFinal.insert(x);
        }

        bool getSfx() { return sfx; }

        bool update_point = false;

        int cur_level = 0;
        std::vector<bool> loaded_level = {false, false, false};
        bool esc_menu = false;

        int score_game = 0;

        bool darker = false;

        std::set<int, std::greater<int>> HighScore;
        std::set<int> MapFinal;

        bool continue_screen = false;
    private:
        Engine() {}
        GameMap* m_LevelMap;
        bool m_isRunning;
        bool sfx = true;
        bool music = true;
        bool m_starting = false;
        bool game_over_screen = false;
        bool high_score_screen = false;
        bool menu_screen = false;
        bool setting_screen = false;
        bool winner = false;
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
        static Engine* s_Instance;
};

#endif // ENGINE_H