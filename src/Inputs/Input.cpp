#include "Input.h"
#include "../Core/Engine.h"

Input* Input::s_Instance = nullptr;

Input::Input() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::Listen() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch (event.type)
        {
        case SDL_QUIT:
            Engine::GetInstance()->Quit();
            break;
        case SDL_KEYDOWN:
            KeyDown();
        case SDL_KEYUP:
            KeyUp();
        default:
            break;
        }
    }
}

bool Input::getKeyDown(SDL_Scancode key) {
    return m_KeyStates[key];
}

void Input::KeyUp() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::KeyDown() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}