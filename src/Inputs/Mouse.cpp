#include "Mouse.h"

Mouse* Mouse::s_Instance = nullptr;

Mouse::Mouse() {
    srect.w = 50;
    srect.h = 50;
    point.x = 1;
    point.y = 1;
    SDL_ShowCursor(false);
}

void Mouse::Load(std::string id, std::string path) {
    TextureManager::GetInstance()->Load(id, path);
}

void Mouse::Update() {
    SDL_GetMouseState(&srect.x, &srect.y);
    point.x = srect.x;
    point.y = srect.y;
    point.w = 16;
    point.h = 16;
}

void Mouse::Draw() {
    TextureManager::GetInstance()->DrawMouse("mouse", 0, 0, 16, 16, srect);
    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect box = {point.x, point.y, 16, 16};
    // box.x -= cam.X;
    // box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
}