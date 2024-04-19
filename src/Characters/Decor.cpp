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

    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(2.0f);
}

void Decor::Draw() {
    for(auto t : m_enemy) {
        t->Draw();
    }
    if(m_type == 5 && portal_show == 0) return;
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);
    // Vector2D cam = Camera::GetInstance()->GetPostision();
    
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
    else if(m_type == 4) {
        if(m_out && (dx == 0 || var == 1)) SetAnimation("sail-no-wind", 8, 100, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale + 60, m_Height * m_scale);
    
        m_RigidBody->UnSetForce();

        m_RigidBody->ApplyForceX(dx * 0.5f);

        if(!m_out && dx != 0 && !var) {
            SetAnimation("sail-wind", 4, 100, 0);
        }
        if(m_out && dx && var) {
            SetAnimation("sail-wind", 4, 100, 0);
        }

        m_RigidBody->Update(dt);
        m_LastSafePosition.X = m_Transform->X;
        m_Transform->X += m_RigidBody->Position().X;
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale + 60, m_Height * m_scale);

        if(var == true) {
            m_TimeVar += dt;
        }

        if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
            m_Transform->X = m_LastSafePosition.X;
            var = 1;
        }  
    }
    else if(m_type == 5) {
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
        if(portal_show == 0) {
            m_respawn += dt;
            if(m_respawn >= 200.0f) {
                m_portal_appear = 1;
                m_respawn = 0;
                spawn_enemy = 0;
            }
            delay_time = 0;
        }
        else {
            if(delay_time == 0) delay_time = SDL_GetTicks();
        }
        if(m_portal_idle) {
            portal_show = 1;
            SetAnimation("monster-portal-idle", 8, 100, delay_time);
            m_time_idle += dt;

            if(m_time_idle >= 50.0f && !spawn_enemy && near_player && (int)m_enemy.size() < 0) {
                Monster* new_enemy = new Monster(new Properties("fish", m_Transform->X, m_Transform->Y, 34, 30, 5));
                new_enemy->Load("fish-idle", "assets/enemy/Sprites/Fierce Tooth/01-Idle/Idle", 8);
                new_enemy->Load("fish-run", "assets/enemy/Sprites/Fierce Tooth/02-Run/Run", 6);
                new_enemy->Load("fish-jump", "assets/enemy/Sprites/Fierce Tooth/03-Jump/Jump", 3);
                new_enemy->Load("fish-fall", "assets/enemy/Sprites/Fierce Tooth/04-Fall/Fall", 1);
                new_enemy->Load("fish-attack", "assets/enemy/Sprites/Fierce Tooth/07-Attack/Attack", 5);
                new_enemy->Load("fish-hit", "assets/enemy/Sprites/Fierce Tooth/08-Hit/Hit", 4);
                new_enemy->Load("fish-dead", "assets/enemy/Sprites/Fierce Tooth/09-Dead Hit/Dead Hit", 4);
                new_enemy->Load("fish-deaded", "assets/enemy/Sprites/Fierce Tooth/10-Dead Ground/Dead Ground", 4);

                new_enemy->setType(4);
                m_enemy.push_back(new_enemy);
                spawn_enemy = true;
            }

            if(m_time_idle >= 200.0f) {
                m_portal_idle = 0;
                m_portal_disappear = 1;
                m_time_idle = 0;
            }
        }
        if(m_portal_appear) {
            portal_show = 1;
            SetAnimation("monster-portal-appear", 8, 120, delay_time);
            m_time_appear += dt;
            if(m_time_appear >= 70.0f) {
                m_portal_idle = 1;
                m_portal_appear = 0;
                m_time_appear = 0;
            }
        }
        if(m_portal_disappear) {
            SetAnimation("monster-portal-disappear", 8, 120, delay_time);
            m_time_disappear += dt;
            if(m_time_disappear >= 70.0f) {
                m_portal_disappear = 0;
                portal_show = 0;
                m_time_disappear = 0;
            }
        }
    }
    AnimationState(dt);
    m_Animation->Update();

    for(auto t : m_enemy) {
        t->Update(dt);
    }
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



