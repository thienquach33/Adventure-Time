#include "Sprites.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/RigidBody.h"
#include "SDL2/SDL.h"
#include "../Camera/Camera.h"
#include "../Core/Engine.h"
#include "../Inputs/Input.h"
#include "../Collision/CollisionHandler.h"
#include "../Collision/TrapCollision.h"
#include "Object.h"
#include "../Core/Engine.h"

Sprites::Sprites(Properties* props) : Character(props) { 
    m_isRunning = false;
    m_isRunning = false;
    m_isFalling = false;
    m_isGrounded = false;
    m_isAttacking = false;
    m_dead = false;

    m_Flip = SDL_FLIP_NONE;
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;
    m_AttackTime = ATTACK_TIME;

    m_Collider = new Collider();
    m_Collider->SetBuffer(0, 0, 0, 0);

    m_Trap = new Collider();
    m_Trap->SetBuffer(0, 0, 0, 0);

    m_Animation = new Animation();
    
    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(4.0f);

    turn_play = 0;

    m_getCoinSound = LoadSound("assets/sounds/item.wav");
    m_jumpSound = LoadSound("assets/sounds/jump.wav");
    m_hurtSound = LoadSound("assets/sounds/hurt.wav");
    m_deadSound = LoadSound("assets/sounds/death.wav");
}

void Sprites::Draw() {
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);
    Vector2D cam = Camera::GetInstance()->GetPostision();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
}

void Sprites::Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip) {
    for(int i = 0; i < num; i++) {
        std::string file_path = path_animation + "-" + std::__cxx11::to_string(i) + ".png";
        std::string name_frame = name_animation + "-" + std::__cxx11::to_string(i);
        TextureManager::GetInstance()->Load(name_frame, file_path);
    }
}

Mix_Chunk* Sprites::LoadSound(const std::string& filePath) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if(sound == nullptr) {
        std::cerr << "Failed to load sound: " << Mix_GetError() << "\n";
    }
    return sound;
}

void Sprites::SetAnimation(std::string animation_name, int num, int speed, int delay_attack) {
    animation_name += "-" + std::__cxx11::to_string(m_Animation->cur_frame);
    m_Animation->setProps(animation_name, num, speed, delay_attack);
}

void Sprites::Respawn(){
    m_Transform->X = 600;
    m_Transform->Y = 600;
    m_dead = false;
    m_DeadTime = 0;
    ++turn_play;
}

void Sprites::Update(double dt) {
    SetAnimation("player-idle", 5, 100, 0);
    m_isRunning = false;
    m_RigidBody->UnSetForce();

    for(auto it = m_heal.begin(); it != m_heal.end(); /* no increment here */) {
        if((*it)->isToBeDestroyed()) {
            it = m_heal.erase(it);
        } else {
            ++it;
        }
    }

    for(auto it = m_box.begin(); it != m_box.end(); /* no increment here */) {
        if((*it)->isToBeDestroyed()) {
            it = m_box.erase(it);
        } else {
            ++it;
        }
    }

    for(auto it = m_item.begin(); it != m_item.end(); /* no increment here */) {
        if((*it)->isToBeDestroyed()) {
            it = m_item.erase(it);
        } else {
            ++it;
        }
    }

    int dx = 0;
    // running
    int run_speed = RUN_FORCE;
    if(m_isPushing) run_speed = PUSH_FORCE;
    if(Input::getInstance()->getAxisKey(HORIZONTAL) == FORWARD && !m_isAttacking) {
        m_RigidBody->ApplyForceX(FORWARD * run_speed);
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_isRunning = true;
        dx = FORWARD;
    }

    if(Input::getInstance()->getAxisKey(HORIZONTAL) == BACKWARD && !m_isAttacking) {
        m_RigidBody->ApplyForceX(BACKWARD * run_speed);
        m_Flip = SDL_FLIP_NONE;
        m_isRunning = true;
        dx = BACKWARD;
    }

    // attacking
    if(Input::getInstance()->getKeyDown(SDL_SCANCODE_J)) {
        m_RigidBody->UnSetForce();
        m_isAttacking = true;
    }

    // jumping
    if(Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && m_isGrounded) {
        Mix_PlayChannel(-1, m_jumpSound, 0);
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

    bool ok = false;

    // move_x
    m_RigidBody->Update(dt);
    m_LastSafePosition.X = m_Transform->X;
    m_Transform->X += m_RigidBody->Position().X;
    m_Collider->Set(m_Transform->X + 22 * 5, m_Transform->Y + 3 * 5, 100, 130);

    for(auto &t : m_box) {
        if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), t->getCollider())) {
            m_Transform->X = m_LastSafePosition.X;
            m_Transform->Y = m_LastSafePosition.Y;

            m_isPushing = true;
            t->applydx(dx);
        }
        else {
            t->applydx(0);
            m_isPushing = false;
        }
        auto sword_collider = m_Collider->Get();
        if(m_Flip == SDL_FLIP_NONE)
            sword_collider.w += 50;
        else 
            sword_collider.x -= 50;
        if(CollisionHandler::GetInstance()->checkCollision(sword_collider, t->getCollider()) && m_isAttacking) {
            t->setHit(true);
        }
        else {
            t->setHit(false);
        }
    }

    // check attack enemy 
    for(auto &t : m_enemy) {
        auto sword_collider = m_Collider->Get();
        if(m_Flip == SDL_FLIP_NONE)
            sword_collider.w += 80;
        else 
            sword_collider.x -= 80;
        if(CollisionHandler::GetInstance()->checkCollision(sword_collider, t->getCollider()) && m_isAttacking) {
            t->setHit(true);
        }
        else {
            t->setHit(false);
        }
    }

    if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
        m_Transform->X = m_LastSafePosition.X;
    }

    m_RigidBody->Update(dt);
    m_LastSafePosition.Y = m_Transform->Y;
    m_Transform->Y += m_RigidBody->Position().Y;
    m_Collider->Set(m_Transform->X + 22 * 5, m_Transform->Y + 3 * 5, 100, 130);

    bool above = false;

    for(auto &t : m_box) {
        if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), t->getCollider())) {
            m_Transform->Y = m_LastSafePosition.Y;
            m_isGrounded = true;
            above = true;
        }
    }

    if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
        m_isGrounded = true;
        m_Transform->Y = m_LastSafePosition.Y;
    }
    else if(!above) {
        m_isGrounded = false;
    }

    // check get coin

    for(auto &t : m_item) {
        if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), t->getCollider())) {
            Mix_PlayChannel(-1, m_getCoinSound, 0);
            t->eat();
        }
    }

    // check game over
    if(TrapCollision::GetInstance()->checkTrapCollision(m_Collider->Get())) {
        m_Transform->Y = m_LastSafePosition.Y;
        m_dead = true;
        m_DeadTime += dt;
    }

    for(int i = 0; i < (int) m_heal.size(); i++) {
        int diff;
        if(m_heal.size() == 3) diff = 35;
        else if(m_heal.size() == 2) diff = 75;
        else diff = 120;
        m_heal[i]->setCollider(m_Transform->X + diff + i * 80, m_Transform->Y - 60, 1, 1);
    }

    m_Origin->x = m_Transform->X + m_Width / 2;
    m_Origin->y = m_Transform->Y + m_Height / 2;

    AnimationState();
    m_Animation->Update();
}

void Sprites::AnimationState() {
    SetAnimation("player-idle", 5, 100, 0);

    if(m_isRunning) 
        SetAnimation("player-run", 6, 100, 0);
    if(m_isJumping)
        SetAnimation("player-jump", 3, 60, 0);
    if(m_isAttacking) {
        if(attackStartTicks == 0) {
            attackStartTicks = SDL_GetTicks();
        }
        SetAnimation("player-attack", 3, 150, attackStartTicks);
        int diff = (m_Flip == SDL_FLIP_NONE) ? 200 : -50;
        m_Effect->setAttack(true);
        m_Effect->SetAnimation("player-attack-effect", m_Transform->X + diff, m_Transform->Y, 3, 200, attackStartTicks, m_Flip);
    }
    else {
        attackStartTicks = 0;
        m_Effect->setAttack(false);
    }
    if(m_dead) {
        if(m_DeadTime >= 100.0f) {
            Respawn();
            if(!m_heal.empty()) m_heal.back()->eat();
        }
        else {
            Mix_PlayChannel(-1, m_deadSound, 0);   
            SetAnimation("player-dead", 4, 150, 0);
        }
    }
}

void Sprites::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
    Mix_FreeChunk(m_getCoinSound);
    Mix_FreeChunk(m_jumpSound);
    Mix_FreeChunk(m_hurtSound);
}



