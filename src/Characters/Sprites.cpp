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
    m_isAttacking = 0;
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
    haveSword = false;
    m_Transform->X = 8 * 80;
    m_Transform->Y = 15 * 80;
    m_dead = false;
    m_DeadTime = 0;
    ++turn_play;
    Engine::GetInstance()->SetGameOver(true);
    Engine::GetInstance()->HighScore.insert(Engine::GetInstance()->score_game);
}

void Sprites::Update(double dt) {
    m_Collider->Set(m_Transform->X + 22 * 5, m_Transform->Y + 3 * 5, 100, 130);
    if(haveSword)
        SetAnimation("player-idle-sw", 5, 100, 0);
    else 
        SetAnimation("player-idle", 5, 100, 0);

    m_isRunning = false;
    m_RigidBody->UnSetForce();

    m_wasGrounded = m_isGrounded;

    if(m_Transform->Y >= 11 * 80 || (m_Transform->X >= 22 * 80 && m_Transform->X <= 24 * 80)) {
        m_JumpForce = 13.0f;
        m_RigidBody->SetGravity(4.0f);
    }

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
    for(auto it = m_bottle.begin(); it != m_bottle.end(); /* no increment here */) {
        if((*it)->isToBeDestroyed()) {
            it = m_bottle.erase(it);
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

    // falling
    if(m_RigidBody->Velocity().Y > 0 && !m_isGrounded) m_isFalling = true;
    else m_isFalling = false;

    // attacking
    if(!m_isFalling && Input::getInstance()->getKeyDown(SDL_SCANCODE_J)) {
        m_RigidBody->UnSetForce();
        m_isAttacking = 1;
    }
    if(!m_isFalling && Input::getInstance()->getKeyDown(SDL_SCANCODE_K)) {
        m_RigidBody->UnSetForce();
        m_isAttacking = 2;
    }
    if(!m_isFalling && Input::getInstance()->getKeyDown(SDL_SCANCODE_L)) {
        m_RigidBody->UnSetForce();
        m_isAttacking = 3;
    }

    // jumping
    if(Input::getInstance()->getKeyDown(SDL_SCANCODE_SPACE) && m_isGrounded && (!(m_is_on_ship && ship->moving()))) {
        int cur_sfx = Mix_PlayChannel(-1, m_jumpSound, 0);
        if(Engine::GetInstance()->getSfx()) {
            Mix_Resume(cur_sfx);
        }
        else {
            Mix_Pause(cur_sfx);
        }
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


    // fix attack
    if(m_isAttacking && m_AttackTime > 0) {
        m_AttackTime -= dt;
    }
    else {
        m_isAttacking = false;
        m_AttackTime = ATTACK_TIME;
    }

    bool ok = false;
    bool above = false;

    if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), ship->getCollider())) {
        if(!CollisionHandler::GetInstance()->mapCollision(ship->getCollider())) {
            m_RigidBody->ApplyForceX(2.0f);
            ship->applydx(run_speed);
        }
        m_Transform->Y = m_LastSafePosition.Y;
        m_isGrounded = true;
        above = true;
        ship->getSail()->setOut(false);
        m_is_on_ship = true;
    }
    else {
        m_is_on_ship = false;
        ship->getSail()->setOut(true);
        ship->applydx(0);
    }

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

    if(m_Transform->X > 79 * 80) {
        monster_portal->setNearPlayer(true);
        for(auto &t : monster_portal->getEnemy()) {
            t->setPlayerPosition(Vector2D(m_Transform->X, m_Transform->Y));
        }
    }
    else {
        monster_portal->setNearPlayer(false);
    }

    for(auto &t : monster_portal->getEnemy()) {
        if(abs(t->getCollider().x - m_Collider->Get().x) < 100 && !t->isToBeDestroyed()) {
            t->setAttack(true);
        }
        else {
            t->setAttack(false);
        }
    }

    for(auto &t : monster_portal->getEnemy()) {
        auto sword_collider = m_Collider->Get();
        if(m_Flip == SDL_FLIP_NONE)
            sword_collider.w += 80;
        else 
            sword_collider.x -= 80;
        auto fish_attack = t->getCollider();
        if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), fish_attack) && (t->getAttack() == true)) {
            m_dead = true;
            break;
        }
        if(CollisionHandler::GetInstance()->checkCollision(sword_collider, t->getCollider()) && m_isAttacking) {
            t->setHit(true);
        }
        else {
            t->setHit(false);
        }
    }

    // check attack enemy 
    for(auto &t : m_enemy) {
        if(t->getType() == 0) {
            auto sword_collider = m_Collider->Get();
            if(m_Flip == SDL_FLIP_NONE)
                sword_collider.w += 80;
            else 
                sword_collider.x -= 80;
            auto crab_attack = t->getCollider();
            crab_attack.x -= 80;
            crab_attack.w += 280;
            if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), crab_attack) && (t->getAttack() == true)) {
                m_dead = true;
                break;
            }
            if(CollisionHandler::GetInstance()->checkCollision(sword_collider, t->getCollider()) && m_isAttacking) {
                t->setHit(true);
            }
            else {
                t->setHit(false);
            }
        }
        else if(t->getType() == 1) {
            if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), t->getCollider())) {
                m_dead = true;
            }
        }
        for(auto bl : t->getBullet()) {
            if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), bl->getCollider())) {
                m_dead = true;
            }
        }
    }

    for(auto &t : bomb) {

        t->active(true);

        if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), t->getExploration()->getCollider())
        && t->getExploration()->getExploring()) {
            m_dead = true;
        }
    }

    if(!m_dead) {

        if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
            m_Transform->X = m_LastSafePosition.X;
        }

        m_RigidBody->Update(dt);
        m_LastSafePosition.Y = m_Transform->Y;
        m_Transform->Y += m_RigidBody->Position().Y;
        m_Collider->Set(m_Transform->X + 22 * 5, m_Transform->Y + 3 * 5, 100, 130);

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
            if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), t->getCollider())) {\
                int cur_sfx = Mix_PlayChannel(-1, m_getCoinSound, 0);
                if(Engine::GetInstance()->getSfx()) {
                    Mix_Resume(cur_sfx);
                }
                else {
                    Mix_Pause(cur_sfx);
                }
                t->eat();
            }
        }
        for(auto &t : m_bottle) {
            if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), t->getCollider())) {
                int cur_sfx = Mix_PlayChannel(-1, m_getCoinSound, 0);
                if(Engine::GetInstance()->getSfx()) {
                    Mix_Resume(cur_sfx);
                }
                else {
                    Mix_Pause(cur_sfx);
                }
                t->eat();
                if(t->getType() == 6) {
                    m_JumpForce = 25.0f;
                    m_RigidBody->SetGravity(6.0f);
                }
            }
        }

        if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), m_sword->getCollider())) {
            haveSword = true;
            m_sword->eat();
        }

        // check game over
        if(TrapCollision::GetInstance()->checkTrapCollision(m_Collider->Get())) {
            m_Transform->Y = m_LastSafePosition.Y;
            m_dead = true;
        }
    }

    if(CollisionHandler::GetInstance()->checkCollision(m_Collider->Get(), m_portal_gate->getCollider())) {
        m_nextLevelTime += dt;
        m_nextLevel = true;
        if(m_nextLevelTime >= 200.0f) {
            m_Transform->X = 45 * 80;
            m_Transform->Y = 7 * 80;
        }
    }
    else {
        m_nextLevel = false;
        m_nextLevelTime = 0;
    }


    if(level_cur == 1) {

        double health_bar_pos = std::max((double) 45, m_Transform->X - 1400);
        health_bar_pos = std::min(health_bar_pos, (double) 3100);

        health_bar->setCollider(health_bar_pos, 30, 1, 1);
    
        for(int i = 0; i < (int) m_heal.size(); i++) {
            m_heal[i]->setCollider(health_bar_pos + 120 + 80 * i, 96, 1, 1);
        }
    }

    m_Origin->x = m_Transform->X + m_Width / 2;
    m_Origin->y = m_Transform->Y + m_Height / 2;

    AnimationState(dt);
    m_Animation->Update();
}

void Sprites::AnimationState(double dt) {
    if(m_dead) {
        m_DeadTime += dt;
        if(m_DeadTime >= 100.0f) {
            Respawn();
            if(!m_heal.empty()) m_heal.back()->eat();
        }
        else {
            Mix_PlayChannel(-1, m_deadSound, 0);   
            SetAnimation("player-attacked", 4, 150, 0);
        }
    }
    if(!haveSword) {
        if(m_isRunning) 
        SetAnimation("player-run", 6, 100, 0);
        if(m_isJumping)
            SetAnimation("player-jump", 3, 60, 0);
        if(m_isFalling) {
            SetAnimation("player-fall", 1, 100, 0);
        }
    }
    else {
        if(m_isRunning) 
            SetAnimation("player-run-sw", 6, 100, 0);
        if(m_isJumping)
            SetAnimation("player-jump-sw", 3, 60, 0);
        if(m_isAttacking) {
            if(attackStartTicks == 0) {
                attackStartTicks = SDL_GetTicks();
            }
            std::string name_attack = "player-attack" + std::to_string(m_isAttacking);
            std::string name_attack_effect = "player-attack-effected" + std::to_string(m_isAttacking);
            SetAnimation(name_attack, 3, 120, attackStartTicks);
            int diff = (m_Flip == SDL_FLIP_NONE) ? 280 : -50;
            if(m_isAttacking == 2) {
                diff = (m_Flip == SDL_FLIP_NONE) ? 250 : -100;
            }
            if(m_isAttacking == 3) {
                diff = (m_Flip == SDL_FLIP_NONE) ? 250 : -100;
            }
            for(auto t : m_Effect) {
                if(t->getName() == name_attack_effect) {
                    double pos_Y = m_Transform->Y + 80;
                    if(m_isAttacking == 2) {
                        pos_Y -= 20;
                    }
                    if(m_isAttacking == 3) {
                        pos_Y -= 70;
                    }
                    t->SetAnimation(name_attack_effect, m_Transform->X + diff, pos_Y, 3, 180, attackStartTicks, m_Flip);
                    t->setAttack(true);
                    t->Update(dt);
                }
                else {
                    t->setAttack(false);
                }
            }
        }
        else {
            for(auto t : m_Effect) {
                t->setAttack(false);
            }
            attackStartTicks = 0;
        }
        if(m_dead) {
            if(m_DeadTime >= 100.0f) {
                Respawn();
                if(!m_heal.empty()) m_heal.back()->eat();
            }
            else {
                int cur_sfx = Mix_PlayChannel(-1, m_deadSound, 0);   
                if(Engine::GetInstance()->getSfx()) {
                    Mix_Resume(cur_sfx);
                }
                else {
                    Mix_Pause(cur_sfx);
                }
                SetAnimation("player-attacked", 4, 150, 0);
            }
        }
        if(m_isFalling) {
            SetAnimation("player-fall-sw", 1, 100, 0);
        }
    }
}

void Sprites::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
    Mix_FreeChunk(m_getCoinSound);
    Mix_FreeChunk(m_jumpSound);
    Mix_FreeChunk(m_hurtSound);
}



