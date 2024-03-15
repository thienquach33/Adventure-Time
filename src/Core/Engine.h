#ifndef ENGINE_H
#define ENGINE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
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

        void Init_Level(int level);

        void Update();
        void Render();
        void Events();

        GameMap* GetMap() { return m_LevelMap; }

        inline bool isRunning() { return m_isRunning; }
        inline  SDL_Renderer* getRenderer() { return m_Renderer; }

        int cur_level = 1;
        bool loaded_level1 = false;
        bool loaded_level2 = false;
        bool loaded_level3 = false;

    private:
        Engine() {}
        GameMap* m_LevelMap;
        bool m_isRunning;
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
        static Engine* s_Instance;
};

#endif // ENGINE_H