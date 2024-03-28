#ifndef MONSTER_H
#define MONSTER_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animation/Animation.h"
#include "../Physics/Collider.h"
#include "Object.h"
#include "item.h"
#include <SDL2/SDL_mixer.h>
#include "effect.h"

#define JUMP_TIME 15.0f
#define JUMP_FORCE 13.0f

#define RUN_FORCE 4.0f
#define ATTACK_TIME 20.0f
#define PUSH_FORCE 2.0f

#define BACKWARD 1
#define FORWARD -1

class Monster : public Character {
    public :    
        Monster(Properties* props);
        virtual void Draw();
        virtual void Update(double dt);
        virtual void Clean();
        virtual void Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip = SDL_FLIP_NONE);
        virtual void SetAnimation(std::string animation_name, int num, int speed, int delay_attack = 0);
        virtual void AnimationState(double dt);
        inline bool getAttack() { return m_isAttacking; }
        virtual void addBox(std::vector<Object*> box) { m_box = box; }
        virtual void addItem(std::vector<Item*> item) { m_item = item; }    
        virtual Mix_Chunk* LoadSound(const std::string& path);
        virtual void Respawn();
        virtual int getTurn() { return turn_play; }
        void addEffect(std::vector<Effect*> attack_effect) { m_Effect = attack_effect; }
        void setType(int type) { this->type = type; }
        void addBall(Object* ball) { m_ball = ball; }
        Object* getBall() { return m_ball; }
        std::vector<Effect*> getEffect() { return m_Effect; }

        int attackStartTicks = 0;

        bool isToBeDestroyed() const {
            return m_tobeDestroy;
        }

        virtual void setHit(bool m_hit) { m_isHitting = m_hit; }

        SDL_Rect getCollider() { return m_Collider->Get(); }
    private :
        enum class State {
            MovingLeft,
            MovingRight,
            IdleLeft,
            IdleRight,
            Attacking_left,
            Attacking_right,
        };

        State m_State = State::MovingLeft;
        double m_IdleTime = 0;
        double m_MoveTime = 0;

        Animation* m_Animation;
        RigidBody* m_RigidBody;

        double m_JumpTime;
        double m_JumpForce;
        double m_AttackTime;
        double m_DeadTime;
        double m_timeAttack = 0, m_recall = 0;
        double m_fire = 0;
        int turn_play, type;

        bool add_point = false;
        bool m_isJumping;
        bool m_isGrounded;
        bool m_isFalling;
        bool m_isRunning;
        bool m_isAttacking;
        bool m_dead;
        bool m_isPushing;
        bool m_tobeDestroy = false;
        bool m_isHitting = false;

        int heal_of_enemy = 500;
        int m_AttackCount = 0;

        std::vector<Effect*> m_Effect;
        Collider* m_Collider;
        Collider* m_Trap;
        std::vector<Object*> m_box;
        Object* m_ball;
        std::vector<Item*> m_item;
        

        Vector2D m_LastSafePosition;
        Vector2D m_lastSafePostion2;
        Mix_Chunk* m_getCoinSound = nullptr;
        Mix_Chunk* m_jumpSound = nullptr;
        Mix_Chunk* m_hurtSound = nullptr;
        Mix_Chunk* m_deadSound = nullptr;
};

#endif // MONSTER_H