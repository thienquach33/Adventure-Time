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

int Input::getAxisKey(Axis axis) {
    if(axis == 0) {
        if(getKeyDown(SDL_SCANCODE_D) || getKeyDown(SDL_SCANCODE_RIGHT)) {
            return 1;
        }
        
        if(getKeyDown(SDL_SCANCODE_A) || getKeyDown(SDL_SCANCODE_LEFT)) {
            return -1;
        }
    }
    else if(axis == 1) {
        if(getKeyDown(SDL_SCANCODE_W) || getKeyDown(SDL_SCANCODE_UP)) 
            return 1;
        
        if(getKeyDown(SDL_SCANCODE_S) || getKeyDown(SDL_SCANCODE_DOWN))
            return -1;
    }
    return 0;   
}

bool Input::getKeyDown(SDL_Scancode key) {
    return m_KeyStates[key];
}

void Input::KeyUp() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::KeyDown() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
    if(m_KeyStates[SDL_SCANCODE_ESCAPE]) {
        Engine::GetInstance()->setMainMenu();
        Engine::GetInstance()->setHighscore();
        Engine::GetInstance()->setSetting();
    }
}