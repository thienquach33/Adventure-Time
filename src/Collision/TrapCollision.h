#ifndef TRAPCOLLISION_H
#define TRAPCOLLISION_H

#include <SDL2/SDL.h>
#include "../Physics/Vector2D.h"
#include "../map/TileLayer.h"
#include "../map/GameMap.h"
#include "../Core/Engine.h"

class TrapCollision {
    public : 
        bool checkCollision(SDL_Rect a, SDL_Rect b);
        bool mapCollision(SDL_Rect a);
        bool checkTrapCollision(SDL_Rect a);

        inline static TrapCollision* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new TrapCollision(); }
    private : 
        TrapCollision();
        TileMap m_CollisionTilemap;
        TileLayer* m_CollisionLayer;
        static TrapCollision* s_Instance;
};

#endif // TRAPCOLLISION_H