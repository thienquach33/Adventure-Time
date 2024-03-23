#include "Decor.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "../Camera/Camera.h"
#include "../Core/Engine.h"
#include "../Inputs/Input.h"
#include "../Collision/CollisionHandler.h"
#include "../Collision/TrapCollision.h"

Decor::Decor(Properties* props) : Character(props) { 
    m_Collider = new Collider();
    m_Collider->SetBuffer(0, 0, 0, 0);
    
    m_Animation = new Animation();
}

void Decor::Draw() {
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);
    Vector2D cam = Camera::GetInstance()->GetPostision();
    // SDL_Rect box = m_Collider->Get();
    // box.x -= cam.X;
    // box.y -= cam.Y;
    // SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
}

void Decor::Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip) {
    for(int i = 0; i < num; i++) {
        std::string file_path = path_animation + "-" + std::__cxx11::to_string(i) + ".png";
        std::string name_frame = name_animation + "-" + std::__cxx11::to_string(i);
        TextureManager::GetInstance()->Load(name_frame, file_path);
    }
}

void Decor::SetAnimation(std::string animation_name, int num, int speed, int delay_attack) {
    animation_name += "-" + std::__cxx11::to_string(m_Animation->cur_frame);
    m_Animation->setProps(animation_name, num, speed, delay_attack);
}

void Decor::Update(double dt) {
    if(m_type == 0) {
        SetAnimation("Flag-idle", 9, 100, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width, m_Height);
    }
    else if(m_type == 1) {
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
        SetAnimation("platform", 1, 100, 0);
    }
    else if(m_type == 2) {
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
        SetAnimation("tree", 4, 100, 0);
    }
    else if(m_type == 3) {
        if(m_isTurning == false) SetAnimation("helm-idle", 6, 100, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
        helm_time_idle += dt;
        if(helm_time_idle >= 80.0f) {
            m_isTurning = true;
            helm_time_idle = 0;
        }
    }
    AnimationState(dt);
    m_Animation->Update();
}

void Decor::AnimationState(double dt) {
    if(m_isTurning) {
        helm_time_turn += dt;
        SetAnimation("helm-turn", 4, 100, 0);
        if(helm_time_turn >= 50.0f) {
            m_isTurning = false;
            helm_time_turn = 0;
            helm_time_idle = 0;
        }
    }
}   

void Decor::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}



