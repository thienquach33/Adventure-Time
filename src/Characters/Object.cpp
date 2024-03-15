#include "Object.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "../Camera/Camera.h"
#include "../Core/Engine.h"
#include "../Inputs/Input.h"
#include "../Collision/CollisionHandler.h"
#include "../Collision/TrapCollision.h"

Object::Object(Properties* props) : Character(props) { 
    m_Collider = new Collider();
    m_Collider->SetBuffer(0, 0, 0, 0);
    
    m_Animation = new Animation();

    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(2.0f);
}

void Object::Draw() {
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);

    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
}

void Object::Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip) {
    for(int i = 0; i < num; i++) {
        std::string file_path = path_animation + "-" + std::__cxx11::to_string(i) + ".png";
        std::string name_frame = name_animation + "-" + std::__cxx11::to_string(i);
        TextureManager::GetInstance()->Load(name_frame, file_path);
    }
}

void Object::SetAnimation(std::string animation_name, int num, int speed, int delay_attack) {
    animation_name += "-" + std::__cxx11::to_string(m_Animation->cur_frame);
    m_Animation->setProps(animation_name, num, speed, delay_attack);
}

void Object::Update(double dt) {
    if(m_type == 0) {
        SetAnimation("box-idle", 1, 100, 0);
        m_RigidBody->UnSetForce();

        m_RigidBody->ApplyForceX(dx * PUSH_FORCE);

        m_RigidBody->Update(dt);
        m_LastSafePosition.X = m_Transform->X;
        m_Transform->X += m_RigidBody->Position().X;
        m_Collider->Set(m_Transform->X, m_Transform->Y, 25 * 5, 17 * 5);

        if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
            m_Transform->X = m_LastSafePosition.X;
        }   


        m_RigidBody->Update(dt);
        m_LastSafePosition.Y = m_Transform->Y;
        m_Transform->Y += m_RigidBody->Position().Y;
        m_Collider->Set(m_Transform->X, m_Transform->Y, 25 * 5, 17 * 5);

        if(heal_of_box == 0) {
            m_RigidBody->SetGravity(1.5f);
            m_DestroyTimer += dt;
            m_Collider->Set(m_Transform->X, m_Transform->Y, 15, 30);
            m_Transform->Y += m_RigidBody->Position().Y;
        }

        if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
            m_isGrounded = true;
            m_Transform->Y = m_LastSafePosition.Y;
        }
        else {
            m_isGrounded = false;
        }

        m_Origin->x = m_Transform->X + m_Width / 2;
        m_Origin->y = m_Transform->Y + m_Height / 2;
    }
    else {
        SetAnimation("portal-gate", 6, 200, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, 20 * 10, 32 * 10);
    }

    AnimationState();
    m_Animation->Update();

}

void Object::AnimationState() {
    if(m_type == 0) {
        if(m_isHitting && heal_of_box >= 10) {
            heal_of_box = std::max(heal_of_box - 10, 0);
            SetAnimation("box-hit", 4, 100, 0);
        }
        if(heal_of_box <= 0) {
            SetAnimation("box-destroy", 5, 200, 0);
            m_scale = 2;
            if(m_DestroyTimer >= 150.0f) {
                m_tobeDestroy = true;
            }
        }
    }
    else {

    }
}

void Object::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}



