#include "effect.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "../Camera/Camera.h"
#include "../Core/Engine.h"
#include "../Inputs/Input.h"
#include "../Collision/CollisionHandler.h"
#include "../Collision/TrapCollision.h"

Effect::Effect(Properties* props) : Character(props) { 
    m_Collider = new Collider();
    m_Collider->SetBuffer(0, 0, 0, 0);
    
    m_Animation = new Animation();
}

void Effect::Draw() {
    if(m_isAttacking) {
        m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, 3, m_Flip);

        Vector2D cam = Camera::GetInstance()->GetPostision();
        SDL_Rect box = m_Collider->Get();
        box.x -= cam.X;
        box.y -= cam.Y;
        SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
    }
}

void Effect::Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip) {
    for(int i = 0; i < num; i++) {
        std::string file_path = path_animation + "-" + std::__cxx11::to_string(i) + ".png";
        std::string name_frame = name_animation + "-" + std::__cxx11::to_string(i);
        TextureManager::GetInstance()->Load(name_frame, file_path);
    }
}

void Effect::SetAnimation(std::string animation_name, int x, int y, int num, int speed, int delay_attack, SDL_RendererFlip flip) {
    animation_name += "-" + std::__cxx11::to_string(m_Animation->cur_frame);
    m_Animation->setProps(animation_name, num, speed, delay_attack);
    m_Transform->X = x;
    m_Transform->Y = y;
    m_Flip = flip;
}

void Effect::Update(double dt) {
    SetAnimation("player-attack-effect", m_Transform->X, m_Transform->Y, 3, 100, 0, m_Flip);
    m_Collider->Set(m_Transform->X, m_Transform->Y, 25 * 5, 17 * 5);
    m_Animation->Update();
}

void Effect::AnimationState() {

}

void Effect::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}
