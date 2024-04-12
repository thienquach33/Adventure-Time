#ifndef MOUSE_H
#define MOUSE_H

#include <SDL2/SDL.h>
#include "../Graphics/TextureManager.h"
#include "../Core/Engine.h"
#include "../Camera/Camera.h"

class Mouse {
    public :
        static Mouse* getInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new Mouse(); }
        virtual void Load(std::string id, std::string path);
        virtual void Draw();
        virtual void Update();

        SDL_Rect getPoint() { return point; }

        bool isClicked() {
            return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
        }
        bool oneClickedCheck() {
            if(isClicked() && !clicked) {
                clicked = true;
                return true;
            }
            else if(!isClicked()) {
                clicked = false;
            }
            return false;
        }
    private :
        bool clicked = false; 
        Mouse();
        SDL_Rect srect, point;
        static Mouse* s_Instance;
};

#endif // MOUSE_H