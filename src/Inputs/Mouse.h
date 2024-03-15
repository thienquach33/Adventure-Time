#ifndef MOUSE_H
#define MOUSE_H

#include <SDL2/SDL.h>
#include "../Graphics/TextureManager.h"

class Mouse {
    public :
        static Mouse* getInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new Mouse(); }
        virtual void Load(std::string id, std::string path);
        virtual void Draw();
        virtual void Update();
    private : 
        Mouse();
        SDL_Rect srect, point;
        static Mouse* s_Instance;
};

#endif // MOUSE_H