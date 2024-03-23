#ifndef ENGINE_H
#define ENGINE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "../map/GameMap.h"

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

        GameMap* GetMap() { return m_LevelMap; }

        inline bool isRunning() { return m_isRunning; }
        inline bool isStarting() { return m_starting; }
        inline  SDL_Renderer* getRenderer() { return m_Renderer; }
    

        void setStarting(bool start) { m_starting = start; }
        void setRunning(bool run) { m_isRunning = run; }

        int cur_level = 0;
        std::vector<bool> loaded_level = {false, false, false};

    private:
        Engine() {}
        GameMap* m_LevelMap;
        bool m_isRunning;
        bool m_starting = false;
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
        static Engine* s_Instance;
};

#endif // ENGINE_H