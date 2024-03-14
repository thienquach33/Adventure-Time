#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include <SDL2/SDL.h>
#include "../Physics/Vector2D.h"
#include "../map/TileLayer.h"
#include "../map/GameMap.h"
#include "../Core/Engine.h"

class CollisionHandler {
    public : 
        bool checkCollision(SDL_Rect a, SDL_Rect b);
        bool checkCollsionX(SDL_Rect a, SDL_Rect b);
        bool checkCollsionY(SDL_Rect a, SDL_Rect b);
        bool mapCollision(SDL_Rect a);

        inline static CollisionHandler* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new CollisionHandler(); }
    private : 
        CollisionHandler();
        TileMap m_CollisionTilemap;
        TileLayer* m_CollisionLayer;
        static CollisionHandler* s_Instance;
};

#endif // COLLISIONHANDLER_H