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
    if(m_type == 3) {
        if(m_exploring) {
            m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);
        }
    }
    else m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);
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
    else if(m_type == 1){
        SetAnimation("portal-gate", 6, 200, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, 20 * 10, 32 * 10);
    }
    else if(m_type == 2) {
        SetAnimation("ball-idle", 1, 100, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);

        if(active_bomb) {

            if(!m_isGrounded) m_RigidBody->ApplyForceY(2.0f);

            if(m_RigidBody->Velocity().Y > 0 && !m_isGrounded) m_isFalling = true;
            else m_isFalling = false;  

            m_RigidBody->Update(dt);
            m_LastSafePosition.Y = m_Transform->Y;
            m_Transform->Y += m_RigidBody->Position().Y;
            m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale); 

            if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
                m_isGrounded = true;
                m_Transform->Y = m_LastSafePosition.Y;
            }
            else {
                m_isGrounded = false;
            }
        }
    }
    else if(m_type == 3) {
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
        SetAnimation("exploring", 7, 100, 0);
    }
    else if(m_type == 4) {
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
        SetAnimation("ship-idle", 6, 100, 0);
    
        m_RigidBody->UnSetForce();

        m_RigidBody->ApplyForceX(dx * 0.5f);
        sail->applydx(dx);

        m_RigidBody->Update(dt);
        m_LastSafePosition.X = m_Transform->X;
        m_Transform->X += m_RigidBody->Position().X;
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);

        if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
            m_Transform->X = m_LastSafePosition.X;
        }  
    }

    AnimationState(dt);
    m_Animation->Update();

}

void Object::AnimationState(double dt) {
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
    else if(m_type == 2) {
        if(m_isGrounded && active_bomb) {
            
            SetAnimation("ball-destroy", 3, 150, 0);
            explore->setExploring(true);
            m_DestroyTimer += dt;
            if(m_DestroyTimer >= 80.0f) {
                explore->setExploring(false);
                m_DestroyTimer = 0;
                respawn(29 * 80, -3 * 80);
            }
        }
    }
    else if(m_type == 3) {
        if(m_exploring) {
            SetAnimation("exploring", 7, 60, 0);
        }
    }
}

void Object::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}



