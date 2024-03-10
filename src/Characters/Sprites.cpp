#include "Sprites.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "../Camera/Camera.h"
#include "../Core/Engine.h"
#include "../Inputs/Input.h"
#include "../Collision/CollisionHandler.h"

Sprites::Sprites(Properties* props) : Character(props) { 
    m_isRunning = false;
    m_isRunning = false;
    m_isFalling = false;
    m_isGrounded = false;
    m_isAttacking = false;

    m_Flip = SDL_FLIP_NONE;
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;
    m_AttackTime = ATTACK_TIME;
    
    m_Collider = new Collider();
    m_Collider->SetBuffer(0, 0, 0, 0);

    m_Animation = new Animation();
    
    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(3.0f);
}

void Sprites::Draw() {
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);

    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    box.x += (18 * 5);
    box.y += (3 * 5);
    SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
}

void Sprites::Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip) {
    for(int i = 0; i < num; i++) {
        std::string file_path = path_animation + "-" + std::__cxx11::to_string(i) + ".png";
        std::string name_frame = name_animation + "-" + std::__cxx11::to_string(i);
        TextureManager::GetInstance()->Load(name_frame, file_path);
    }
}

void Sprites::SetAnimation(std::string animation_name, int num, int speed) {
    animation_name += "-" + std::__cxx11::to_string(m_Animation->cur_frame);
    m_Animation->setProps(animation_name, num, speed);
}

void Sprites::Update(double dt) {
    SetAnimation("player-idle", 5, 100);
    m_isRunning = false;
    m_RigidBody->UnSetForce();

    // running
    if(Input::getInstance()->getAxisKey(HORIZONTAL) == FORWARD && !m_isAttacking) {
        m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_NONE;
        m_isRunning = true;
    }

    if(Input::getInstance()->getAxisKey(HORIZONTAL) == BACKWARD && !m_isAttacking) {
        m_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_isRunning = true;
    }

    // attacking
    if(Input::getInstance()->getKeyDown(SDL_SCANCODE_J)) {
        m_RigidBody->UnSetForce();
        m_isAttacking = true;
    }

    // jumping
    if(Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && m_isGrounded) {
        m_isJumping = true;
        m_isGrounded = false;
        m_RigidBody->ApplyForceY(FORWARD * m_JumpForce);
    }
    if(Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && m_isJumping && m_JumpTime > 0){
        m_JumpTime -= dt;
        m_RigidBody->ApplyForceY(FORWARD * m_JumpForce);
    }
    else {
        m_isJumping = false;
        m_JumpTime = JUMP_TIME;
    }
    m_RigidBody->Update(dt);
    
    // falling
    if(m_RigidBody->Velocity().Y > 0 && !m_isGrounded) m_isFalling = true;
    else m_isFalling = false;

    // fix attack
    if(m_isAttacking && m_AttackTime > 0) {
        m_AttackTime -= dt;
    }
    else {
        m_isAttacking = false;
        m_AttackTime = ATTACK_TIME;
    }

    // move_x
    m_RigidBody->Update(dt);
    m_LastSafePosition.X = m_Transform->X;
    m_Transform->X += m_RigidBody->Position().X;
    m_Collider->Set(m_Transform->X, m_Transform->Y, 200, 145);
    
    if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
        m_Transform->X = m_LastSafePosition.X;
    }

    // move y
    m_RigidBody->Update(dt);
    m_LastSafePosition.Y = m_Transform->Y;
    m_Transform->Y += m_RigidBody->Position().Y;
    m_Collider->Set(m_Transform->X, m_Transform->Y, 200, 145);

    if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
        m_isGrounded = true;
        m_Transform->Y = m_LastSafePosition.Y;
    } 
    else {
        m_isGrounded = false;
    }

    m_Origin->x = m_Transform->X + m_Width / 2;
    m_Origin->y = m_Transform->Y + m_Height / 2;
    m_Animation->Update();
    AnimationState();
}

void Sprites::AnimationState() {
    SetAnimation("player-idle", 5, 100);

    if(m_isRunning) 
        SetAnimation("player-run", 6, 100);
    if(m_isJumping)
        SetAnimation("player-jump", 3, 100);
    if(m_isAttacking)
        SetAnimation("player-attack", 3, 100);
}

void Sprites::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}



