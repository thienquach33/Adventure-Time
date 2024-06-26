#include "Monster.h"
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

struct Node {
    int du, u, v;

    bool operator < (const Node &others) const {
        return (du > others.du);
    }
};

std::vector<std::pair<int, int>> Monster::Dijkstra() {
    std::vector<std::pair<int, int>> path;

    std::priority_queue<Node> pq;

    TileMap grid = CollisionHandler::GetInstance()->GetCollisionTilemap();

    std::vector<std::vector<int>> dp(grid.size(), std::vector<int>(grid[0].size(), (int) 1e9 + 7));
    std::vector<std::vector<std::pair<int, int>>> pre(grid.size(), std::vector<std::pair<int, int>>(grid[0].size()));

    for(int i = 0; i < (int) grid.size(); i++) {
        for(int j = 0; j < (int) grid[0].size(); j++) {
            pre[i][j] = {-1, -1};
        }
    }

    dp[m_Transform->Y / 80][m_Transform->X / 80] = 0;
    pq.push({dp[(int) m_Transform->Y / 80][(int) m_Transform->X / 80], (int) m_Transform->Y / 80, (int) m_Transform->X / 80});

    while(!pq.empty()) {
        auto cur = pq.top();
        pq.pop();

        if(cur.du != dp[cur.u][cur.v]) continue;

        if(cur.u == (int) player_position.Y / 80 && cur.v == (int) player_position.X / 80) break;

        std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (auto dir : directions) {
            int u = cur.u + dir.first;
            int v = cur.v + dir.second;

            if(u < 0 || u >= (int) grid.size() || v < 0 || v >= (int) grid[0].size()) continue;

            if(grid[u][v] == 1) continue;

            if(dp[u][v] > dp[cur.u][cur.v] + 1) {
                dp[u][v] = dp[cur.u][cur.v] + 1;
                pre[u][v] = {cur.u, cur.v};
                pq.push({dp[u][v], u, v});
            }
        }
    }

    std::pair<int, int> cur = {player_position.Y / 80, player_position.X / 80};
    while(pre[cur.first][cur.second].first != -1 && pre[cur.first][cur.second].second != -1) {
        path.push_back(cur);
        cur = pre[cur.first][cur.second];
    }

    std::reverse(path.begin(), path.end());

    return path;
}

Monster::Monster(Properties* props) : Character(props) { 
    m_isRunning = false;
    m_isRunning = false;
    m_isFalling = false;
    m_isGrounded = false;
    m_isAttacking = false;
    m_dead = false;

    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;
    m_AttackTime = ATTACK_TIME;

    m_Collider = new Collider();
    m_Collider->SetBuffer(0, 0, 0, 0);

    m_Trap = new Collider();
    m_Trap->SetBuffer(0, 0, 0, 0);

    m_Animation = new Animation();
    
    m_RigidBody = new RigidBody();
    m_RigidBody->SetGravity(6.0f);
}

void Monster::Draw() {
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_scale, m_Flip);


    // Vector2D cam = Camera::GetInstance()->GetPostision();
    // SDL_Rect box = m_Collider->Get();
    // box.x -= cam.X;
    // box.y -= cam.Y;
    // SDL_RenderDrawRect(Engine::GetInstance()->getRenderer(), &box);
    

    for(auto t : bullet) {
        t->Draw();
    }
}

void Monster::Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip) {
    for(int i = 0; i < num; i++) {
        std::string file_path = path_animation + "-" + std::__cxx11::to_string(i) + ".png";
        std::string name_frame = name_animation + "-" + std::__cxx11::to_string(i);
        TextureManager::GetInstance()->Load(name_frame, file_path);
    }
}

Mix_Chunk* Monster::LoadSound(const std::string& filePath) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if(sound == nullptr) {
        std::cerr << "Failed to load sound: " << Mix_GetError() << "\n";
    }
    return sound;
}

void Monster::SetAnimation(std::string animation_name, int num, int speed, int delay_attack) {
    animation_name += "-" + std::__cxx11::to_string(m_Animation->cur_frame);
    m_Animation->setProps(animation_name, num, speed, delay_attack);
}

void Monster::Respawn(){
    m_Transform->X = 600;
    m_Transform->Y = 600;
    m_dead = false;
    m_DeadTime = 0;
    ++turn_play;
}

void Monster::Update(double dt) {
    if(type == 0) {
        SetAnimation("enemy-idle", 8, 100, 0);
        m_isRunning = false;
        m_RigidBody->UnSetForce();

        if(!m_isFalling && !m_dead && !m_isHitting && heal_of_enemy > 0) {

            switch(m_State) {
                case State::MovingLeft:
                    m_isRunning = true;
                    m_RigidBody->ApplyForceX(FORWARD * 2.0f); // Adjust the force as needed
                    m_MoveTime += dt;
                    if(m_MoveTime >= 70.0f) { // Adjust the time as needed
                        m_State = State::IdleLeft;
                        m_MoveTime = 0.0;
                        m_Flip = SDL_FLIP_HORIZONTAL;
                    }
                    break;
                case State::MovingRight:
                    m_isRunning = true;
                    m_RigidBody->ApplyForceX(BACKWARD * 2.0f); // Adjust the force as needed
                    m_MoveTime += dt;
                    if(m_MoveTime >= 70.0f) { // Adjust the time as needed
                        m_State = State::IdleRight;
                        m_MoveTime = 0.0;
                        m_Flip = SDL_FLIP_NONE;
                    }
                    break;
                case State::IdleLeft:
                    m_IdleTime += dt;
                    if(m_IdleTime >= 80.0f) { // Adjust the time as needed
                        m_State = State::Attacking_left;
                        m_IdleTime = 0.0;
                    }
                    break;
                case State::IdleRight:
                    m_IdleTime += dt;
                    if(m_IdleTime >= 80.0f) { // Adjust the time as needed
                        m_State = State::Attacking_right;
                        m_IdleTime = 0.0;
                    }
                    break;
                case State::Attacking_left :
                    m_isAttacking = true;
                    m_timeAttack += dt;
                    if(m_timeAttack >= 80.0f) {
                        m_isAttacking = false;
                        m_timeAttack = 0;
                    }
                    if(m_isAttacking && m_AttackTime > 0) {
                        m_AttackTime -= dt;
                    }
                    else {
                        m_isAttacking = false;
                        m_AttackTime = ATTACK_TIME;
                        m_State = State::MovingRight;
                    }
                    break;
                case State::Attacking_right :
                    m_isAttacking = true;
                    m_timeAttack += dt;
                    if(m_timeAttack >= 80.0f) {
                        m_isAttacking = false;
                        m_timeAttack = 0;
                    }
                    if(m_isAttacking && m_AttackTime > 0) {
                        m_AttackTime -= dt;
                    }
                    else {
                        m_isAttacking = false;
                        m_AttackTime = ATTACK_TIME;
                        m_State = State::MovingLeft;
                    }
                    break;
            }
        }
    }
    else if(type == 1) {
        SetAnimation("pink-star-idle", 8, 100);
        m_RigidBody->UnSetForce();
        m_isRunning = false;
        if(!m_isFalling && !m_dead && !m_isHitting && heal_of_enemy > 0) {
            switch(m_State) {
            case State::IdleLeft:
                m_IdleTime += dt;
                if(m_IdleTime >= 120.0f) { // Adjust the time as needed
                    m_State = State::MovingLeft;
                    m_IdleTime = 0.0;
                }
                break;
            case State::IdleRight:
                m_IdleTime += dt;
                if(m_IdleTime >= 120.0f) { // Adjust the time as needed
                    m_State = State::MovingRight;
                    m_IdleTime = 0.0;
                }
                break;
            case State::MovingLeft:
                m_isRunning = true;
                m_RigidBody->ApplyForceX(FORWARD * 2.0f); // Adjust the force as needed
                m_MoveTime += dt;
                if(m_MoveTime >= 120.0f) { // Adjust the time as needed
                    m_State = State::IdleRight;
                    m_MoveTime = 0.0;
                    m_Flip = SDL_FLIP_HORIZONTAL;
                }
                break;
            case State::MovingRight:
                m_isRunning = true;
                m_RigidBody->ApplyForceX(BACKWARD * 2.0f); // Adjust the force as needed
                m_MoveTime += dt;
                if(m_MoveTime >= 120.0f) { // Adjust the time as needed
                    m_State = State::IdleLeft;
                    m_MoveTime = 0.0;
                    m_Flip = SDL_FLIP_NONE;
                }
                break;
            }
        }
    }
    else if(type == 2) {
        SetAnimation("totem-attack", 6, 120, 0);
        
        if(m_AttackTime >= 200.0f) {
            Monster* wood_spike = new Monster(new Properties("wood-spike", m_Transform->X + 30, m_Transform->Y + 80, 16, 16, 3));
            wood_spike->Load("wood-spike-idle", "assets/enemy/Sprites/Totems/Wood Spike/Idle/idle", 1);
            wood_spike->setType(3);
            bullet.push_back(wood_spike);
            m_AttackTime = 0;
        }
        m_AttackTime += dt;
    }
    else if(type == 5) {
        m_Flip = SDL_FLIP_HORIZONTAL;
        SetAnimation("totem-2-attack", 6, 120, 0);
        
        if(m_AttackTime >= 200.0f) {
            Monster* wood_spike = new Monster(new Properties("wood-spike", m_Transform->X + 30, m_Transform->Y + 80, 16, 16, 3));
            wood_spike->Load("wood-spike-idle", "assets/enemy/Sprites/Totems/Wood Spike/Idle/idle", 1);
            wood_spike->setType(6);
            bullet.push_back(wood_spike);
            m_AttackTime = 0;
        }
        m_AttackTime += dt;
    }
    else if(type == 3) {
        m_RigidBody->UnSetForce();
        SetAnimation("wood-spike-idle", 1, 100, 0);
        m_RigidBody->ApplyForceX(FORWARD * 3.5f); // Adjust the force as needed
        m_AttackTime += dt;
        if(m_AttackTime >= 200.0f) {
            m_tobeDestroy = true;
            m_AttackTime = 0;
        }
    }
    else if(type == 6) {
        m_RigidBody->UnSetForce();
        SetAnimation("wood-spike-idle", 1, 100, 0);
        m_RigidBody->ApplyForceX(BACKWARD * 3.5f); // Adjust the force as needed
        m_AttackTime += dt;
        if(m_AttackTime >= 200.0f) {
            m_tobeDestroy = true;
            m_AttackTime = 0;
        }
    }
    else if(type == 4) {
        SetAnimation("fish-idle", 8, 100, 0);
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
        m_RigidBody->UnSetForce();
        m_isRunning = false;

        if(player_position.X != 0 && player_position.Y != 0 && !m_dead && !m_isAttacking && !m_isHitting && heal_of_enemy > 0) {
            std::vector<std::pair<int, int>> path = Dijkstra();
            
            if(path.size() >= 1) {

                std::pair<int, int> nextPos = ((int) path.size() == 1 ? path[0] : path[1]);

                if(m_Transform->Y < player_position.Y && m_Transform->X > (nextPos.second - 1)  * 80) {
                    m_RigidBody->ApplyForceX(FORWARD * 4.0f);
                    m_isRunning = true;
                    m_Flip = SDL_FLIP_NONE;
                }
                else if(m_Transform->X > nextPos.second * 80) {
                    m_RigidBody->ApplyForceX(FORWARD * 4.0f);
                    m_isRunning = true;
                    m_Flip = SDL_FLIP_NONE;
                }

                else if(m_Transform->X < nextPos.second * 80) {
                    m_RigidBody->ApplyForceX(BACKWARD * 4.0f);
                    m_isRunning = true;
                    m_Flip = SDL_FLIP_HORIZONTAL;
                }
            }
        }
    }
    if(m_RigidBody->Velocity().Y > 0 && !m_isGrounded) m_isFalling = true;
    else m_isFalling = false;

    if(type != 3 && type != 6) {

        // move_x
        m_RigidBody->Update(dt);
        m_LastSafePosition.X = m_Transform->X;
        m_Transform->X += m_RigidBody->Position().X;
        int w = 45 * 5;

        if(type == 4 || type == 5 || type == 1) {
            w = 20 * 5;
        }
        m_Collider->Set(m_Transform->X + 13 * 5, m_Transform->Y + 5 * 5, w, 20 * 5);

        if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
            m_Transform->X = m_LastSafePosition.X;
        }

        m_RigidBody->Update(dt);
        m_LastSafePosition.Y = m_Transform->Y;
        m_Transform->Y += m_RigidBody->Position().Y;
        m_Collider->Set(m_Transform->X + 13 * 5, m_Transform->Y + 5 * 5, w, 20 * 5);

        if(CollisionHandler::GetInstance()->mapCollision(m_Collider->Get())) {
            m_isGrounded = true;
            m_Transform->Y = m_LastSafePosition.Y;
        }
        else {
            m_isGrounded = false;
        }
    }
    else {
        m_RigidBody->SetGravity(0);
        m_RigidBody->Update(dt);
        m_LastSafePosition.X = m_Transform->X;
        m_Transform->X += m_RigidBody->Position().X;
        m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
    }

    if(heal_of_enemy == 0) {
        m_DeadTime += dt;
    }

    m_Origin->x = m_Transform->X + m_Width / 2;
    m_Origin->y = m_Transform->Y + m_Height / 2;

    AnimationState(dt);
    m_Animation->Update();

    for(auto it = bullet.begin(); it != bullet.end(); /* no increment here */) {
        if((*it)->m_tobeDestroy) {
            delete(*it);
            it = bullet.erase(it);
        } else {
            ++it;
        }
    }

    for(auto t : bullet) {
        t->Update(dt);
    }
}

void Monster::AnimationState(double dt) {
    if(type == 0) {
        if(m_isRunning) {
            SetAnimation("enemy-run", 6, 100, 0);
        }
        if(m_isHitting) {
            if(heal_of_enemy >= 10) {
                heal_of_enemy = std::max(heal_of_enemy - 10, 0);
                int cur = SDL_GetTicks();
                SetAnimation("enemy-hit", 4, 200, cur);
            }
        }
        if(m_isAttacking) {
            if(attackStartTicks == 0) {
                attackStartTicks = SDL_GetTicks();
            }
            SetAnimation("enemy-attack", 4, 150, attackStartTicks);
            for(auto t : m_Effect) {
                if(t->getName() == "crab-attack-effected") {
                    t->SetAnimation("crab-attack-effected", m_Transform->X - 80, m_Transform->Y + 50, 3, 180, attackStartTicks, m_Flip);
                    t->setAttack(true);
                    t->Update(dt);
                }
            }
        }
        else {
            for(auto t : m_Effect) {
                t->setAttack(false);
            }
            attackStartTicks = 0;
        }
        if(heal_of_enemy == 0) {
            if(m_DeadTime >= 200.0f) {
                m_tobeDestroy = true;
                SetAnimation("enemy-deaded", 4, 200, 0);    
            }
            else {
                if(add_point == false) {
                    Engine::GetInstance()->score_game += 100;
                    add_point = true;
                }
                SetAnimation("enemy-dead", 4, 200, 0);
            }
        }
    }
    else if(type == 1) {
        if(m_isRunning) {
            SetAnimation("pink-star-attack", 4, 100);
            m_Collider->Set(m_Transform->X, m_Transform->Y, m_Width * m_scale, m_Height * m_scale);
        }
        if(m_isHitting) {
            if(heal_of_enemy >= 10) {
                heal_of_enemy = std::max(heal_of_enemy - 10, 0);
                int cur = SDL_GetTicks();
                SetAnimation("pink-star-hit", 4, 200, cur);
            }
        }
        if(heal_of_enemy == 0) {
            if(m_DeadTime >= 200.0f) {
                m_tobeDestroy = true;
                SetAnimation("pink-star-deaded", 4, 200, 0);    
            }
            else {
                if(add_point == false) {
                    Engine::GetInstance()->score_game += 200;
                    add_point = true;
                }
                SetAnimation("pink-star-dead", 4, 200, 0);
            }
        }
    }
    else if(type == 4) {
        if(m_isRunning) {
            SetAnimation("fish-run", 6, 100);
        }
        if(m_isHitting) {
            if(heal_of_enemy >= 10) {
                heal_of_enemy = std::max(heal_of_enemy - 10, 0);
                int cur = SDL_GetTicks();
                SetAnimation("fish-hit", 4, 200, cur);
            }
        }
        if(m_isAttacking) {
            if(attackStartTicks == 0) {
                attackStartTicks = SDL_GetTicks();
            }
            SetAnimation("fish-attack", 4, 150, attackStartTicks);
        }
        else {
            for(auto t : m_Effect) {
                t->setAttack(false);
            }
            attackStartTicks = 0;
        }
        if(heal_of_enemy == 0) {
            if(m_DeadTime >= 200.0f) {
                m_tobeDestroy = true;
                SetAnimation("fish-deaded", 4, 200, 0);    
            }
            else {
                if(add_point == false) {
                    Engine::GetInstance()->score_game += 200;
                    add_point = true;
                }
                SetAnimation("fish-dead", 4, 200, 0);
            }
        }
    }
}

void Monster::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
    Mix_FreeChunk(m_getCoinSound);
    Mix_FreeChunk(m_jumpSound);
    Mix_FreeChunk(m_hurtSound);
}



