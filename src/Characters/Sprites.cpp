#include "Sprites.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "../Camera/Camera.h"
#include "../Core/Engine.h"

Sprites::Sprites(Properties* props) : Character(props) { 
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;
    
    m_Collider = new Collider();
    m_Collider->SetBuffer(0, 0, 0, 0);

    m_Animation = new Animation();
    
    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(3.0f);
}

void Sprites::Draw() {
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale);

    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    std::cerr << box.x << " " << box.y << "\n";
    SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
}

void Sprites::Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip) {
    for(int i = 0; i < num; i++) {
        std::string file_path = path_animation + "-" + std::__cxx11::to_string(i) + ".png";
        std::string name_frame = name_animation + "-" + std::__cxx11::to_string(i);
        TextureManager::GetInstance()->Load(name_frame, file_path);
    }
}

void Sprites::SetAnimation(std::string animation_name, int num, int speed, SDL_RendererFlip flip) {
    animation_name += "-" + std::__cxx11::to_string(m_Animation->cur_frame);
    m_Animation->setProps(animation_name, num, speed, flip);
}

void Sprites::Update(double dt) {
    SetAnimation("player-idle", 5, 80);
    m_RigidBody->UnSetForce();

    if(Input::getInstance()->getKeyDown(SDL_SCANCODE_A)) {
        m_RigidBody->ApplyForceX(-6);
        SetAnimation("player-run", 6, 80, SDL_FLIP_HORIZONTAL);
    }
    if(Input::getInstance()->getKeyDown(SDL_SCANCODE_D)) {
        m_RigidBody->ApplyForceX(6);
        SetAnimation("player-run", 6, 80);
    }
    if(Input::getInstance()->getKeyDown(SDL_SCANCODE_J)) {
        SetAnimation("player-attack", 3, 200);
    }

    m_RigidBody->Update(dt);
    m_Transform->moveX(m_RigidBody->Position().X);

    m_Origin->x = m_Transform->X + m_Width / 2;
    m_Origin->y = m_Transform->Y + m_Height / 2;
    m_Animation->Update();
}

void Sprites::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}



