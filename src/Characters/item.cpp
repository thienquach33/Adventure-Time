#include "item.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "../Camera/Camera.h"
#include "../Core/Engine.h"
#include "../Inputs/Input.h"
#include "../Collision/CollisionHandler.h"
#include "../Collision/TrapCollision.h"

Item::Item(Properties* props) : Character(props) { 
    m_Collider = new Collider();
    m_Collider->SetBuffer(0, 0, 0, 0);
    
    m_Animation = new Animation();
    m_isEtten = false;
}

void Item::Draw() {
    if(m_type >= 9 && !isShow) return;
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);

    // Vector2D cam = Camera::GetInstance()->GetPostision();
    // SDL_Rect box = m_Collider->Get();
    // box.x -= cam.X;
    // box.y -= cam.Y;
    // SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
}

void Item::Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip) {
    for(int i = 0; i < num; i++) {
        std::string file_path = path_animation + "-" + std::__cxx11::to_string(i) + ".png";
        std::string name_frame = name_animation + "-" + std::__cxx11::to_string(i);
        TextureManager::GetInstance()->Load(name_frame, file_path);
    }
}

void Item::SetAnimation(std::string animation_name, int num, int speed, int delay_attack) {
    animation_name += "-" + std::__cxx11::to_string(m_Animation->cur_frame);
    m_Animation->setProps(animation_name, num, speed, delay_attack);
}

void Item::Update(double dt) {
    if(m_type == 0) {
        SetAnimation("sliver-coin", 4, 100, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, 16 * 5, 16 * 5);

        if(m_isEtten) {
            m_EatTimer += dt;
        }
    }
    else if(m_type == 1){
        m_Transform->X = m_Collider->Get().x;
        m_Transform->Y = m_Collider->Get().y;
        SetAnimation("heal-full", 1, 100, 0);
    }
    else if(m_type == 2) {
        SetAnimation("gold-coin", 4, 100, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, 16 * 5, 16 * 5);

        if(m_isEtten) {
            m_EatTimer += dt;
        }
    }
    else if(m_type == 3) {
        m_Transform->X = m_Collider->Get().x;
        m_Transform->Y = m_Collider->Get().y;
        SetAnimation("health-bar", 1, 100, 0);
    }
    else if(m_type == 4) {
        SetAnimation("key-level", 8, 100, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, 24 * 5, 24 * 5);
    }
    else if(m_type == 5) {
        SetAnimation("sword", 8, 100);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
    }
    else if(m_type == 6) {
        SetAnimation("green-bottle", 7, 100);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);

        if(m_isEtten) {
            m_EatTimer += dt;
        }
    }
    else if(m_type == 7) {
        SetAnimation("chest-idle", 1, 100);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
    }
    else if(m_type == 8) {
        SetAnimation("red-diamod", 4, 100);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);

        if(m_isEtten) {
            m_EatTimer += dt;
        }
    }
    else if(m_type >= 9) {
        std::string name_map = "map-" + std::to_string(m_type - 9);
        SetAnimation(name_map, 1, 100);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);

        if(m_isEtten) {
            m_EatTimer += dt;
        }
    }

    AnimationState(dt);
    m_Animation->Update();

}

void Item::AnimationState(double dt) {
    if(m_type == 0 || m_type == 2 || m_type == 6 || m_type == 8 || m_type >= 9) {
        if(m_isEtten) {
            if(add_point == false) {
                if(m_type == 0) Engine::GetInstance()->score_game += 10;
                else if(m_type == 8)  Engine::GetInstance()->score_game += 80;
                else Engine::GetInstance()->score_game += 30;
                add_point = true;
            }
            SetAnimation("coin-effect", 3, 200);
            if(m_EatTimer >= 50.0f) {
                if(m_type >= 9) Engine::GetInstance()->InsertMap(m_type);
                m_tobeDestroy = true;
                m_isEtten = false;
            }
        }
    }
    else if(m_type == 7) {
        if(m_isEtten) {
            m_EatTimer += dt;
            SetAnimation("chest-unlock", 7, 120);
            if(m_EatTimer >= 30.0f) {
                unlock4 = true;
                m_isEtten = false;
            }
        }
        if(unlock4) {
            map->setShow(true);
            SetAnimation("chest-unlock4", 1, 120);
        }
    }
    else {
        if(m_isEtten) {
            m_tobeDestroy = true;
            m_isEtten = false;
        }
    }
}

void Item::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}



