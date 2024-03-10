#include "../Graphics/TextureManager.h"
#include "Animation.h"

Animation::Animation() {

}

void Animation::Update() {
    cur_frame = (SDL_GetTicks() / animation_speed) % num_frame;
}

void Animation::Draw(double x, double y, int w, int h, int sc, SDL_RendererFlip flip) {
    std::string name_frame = m_id;
    name_frame.pop_back();
    name_frame += std::__cxx11::to_string(cur_frame);
    TextureManager::GetInstance()->Draw(name_frame, x, y, w, h, sc, flip);
}
    
void Animation::setProps(std::string id, int num, int speed) {
    m_id = id;
    num_frame = num;
    animation_speed = speed;
}