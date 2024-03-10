#include "Monster.h"
#include "../Graphics/TextureManager.h"
#include "SDL2/SDL.h"

Monster::Monster(Properties* props) : Character(props) { 
    m_FrameCount = 8;
    m_AnimationSpeed = 80;

    // for(int i = 0; i < m_FrameCount; i++) {
    //     std::string file_path = m_Path + std::__cxx11::to_string(i) + ".png";
    //     std::string name_frame = "monster_animation_idle-" + std::__cxx11::to_string(i);
    //     TextureManager::GetInstance()->Load(name_frame, file_path);
    // }
}

void Monster::Draw() {
    // std::string name_frame = "monster_animation_idle-" + std::__cxx11::to_string(m_Frame);
    // TextureManager::GetInstance()->Draw(name_frame, m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale);
}

void Monster::Update(double dt) {
    m_Frame = (SDL_GetTicks() / m_AnimationSpeed) % m_FrameCount;
}

void Monster::Clean() {
    TextureManager::GetInstance()->Clean();
}