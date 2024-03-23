#include "Button.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "../Camera/Camera.h"
#include "../Core/Engine.h"
#include "../Inputs/Input.h"
#include "../Collision/CollisionHandler.h"
#include "../Collision/TrapCollision.h"

Button::Button(Properties* props) : Character(props) { 
    m_Collider = new Collider();
    m_Collider->SetBuffer(0, 0, 0, 0);
    
    m_Animation = new Animation();
}

void Button::Draw() {
    m_Animation->DrawButton(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);

    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
}

void Button::Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip) {
    for(int i = 0; i < num; i++) {
        std::string file_path = path_animation + "-" + std::__cxx11::to_string(i) + ".png";
        std::string name_frame = name_animation + "-" + std::__cxx11::to_string(i);
        TextureManager::GetInstance()->Load(name_frame, file_path);
    }
}

void Button::SetAnimation(std::string animation_name, int num, int speed, int delay_attack) {
    animation_name += "-" + std::__cxx11::to_string(m_Animation->cur_frame);
    m_Animation->setProps(animation_name, num, speed, delay_attack);
}

void Button::Update(double dt) {
    std::string name_button;
    if(type == 0) name_button = "button-play";
    if(type == 1) name_button = "button-setting";
    if(type == 2) name_button = "button-menu";
    if(type == 3) name_button = "button-exit";

    m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
    SDL_Rect temp = m_Collider->Get();
    SDL_Rect temp_mouse = Mouse::getInstance()->getPoint();
    if(SDL_HasIntersection(&temp, &temp_mouse)) {
        if(Mouse::getInstance()->isClicked()) {
            SetAnimation(name_button, 3, 100, 0);
            if(type == 0) {
                Engine::GetInstance()->setStarting(true);
            }
            else if(type == 3) {
                Engine::GetInstance()->setRunning(false);
            
            }
        }
        else { 
            SetAnimation(name_button, 1, 100, 0);
        }
    }
    else {
        SetAnimation(name_button, 1, 100, 0);
    }

    AnimationState();
    m_Animation->Update();

}

void Button::AnimationState() {
    
}

void Button::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}
