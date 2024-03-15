#include "Mouse.h"

Mouse* Mouse::s_Instance = nullptr;

Mouse::Mouse() {
    srect.w = 40;
    srect.h = 40;
    point.x = 1;
    point.y = 1;
    SDL_ShowCursor(false);
}

void Mouse::Load(std::string id, std::string path) {
    TextureManager::GetInstance()->Load(id, path);
}

void Mouse::Mouse::Update() {
    SDL_GetMouseState(&srect.x, &srect.y);
    point.x = srect.x;
    point.y = srect.y;
}

void Mouse::Draw() {
    TextureManager::GetInstance()->DrawMouse("mouse", 0, 0, 16, 16, srect);
}