#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

class Input {
    public :
        static Input* getInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new Input(); }

        void Listen();
        bool getKeyDown(SDL_Scancode key);
    private :
        Input();
        void KeyUp();
        void KeyDown();

        const Uint8* m_KeyStates;
        static Input* s_Instance;
};

#endif // INPUT_H