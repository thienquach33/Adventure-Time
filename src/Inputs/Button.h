#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>

class Button {
    public :
        SDL_Texture* tex;
        SDL_Rect srect, drect;
        bool isSelected = false;

        Button();
        virtual ~Button();

        void Update(Mouse)
};

#endif // BUTTON_H