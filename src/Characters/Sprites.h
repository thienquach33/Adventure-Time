#ifndef SPRITES_H
#define SPRITES_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animation/Animation.h"
#include "../Physics/Collider.h"
#include "Object.h"
#include "item.h"
#include "effect.h"
#include <SDL2/SDL_mixer.h>
#include "Monster.h"

#define JUMP_TIME 15.0f
#define JUMP_FORCE 13.0f

#define RUN_FORCE 4.0f
#define ATTACK_TIME 20.0f
#define PUSH_FORCE 2.0f

#define BACKWARD 1
#define FORWARD -1

class Sprites : public Character {
    public :    
        Sprites(Properties* props);
        virtual void Draw();
        virtual void Update(double dt);
        virtual void Clean();
        virtual void Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip = SDL_FLIP_NONE);
        virtual void SetAnimation(std::string animation_name, int num, int speed, int delay_attack = 0);
        virtual void AnimationState();
        inline int getAttack() { return m_isAttacking; }
        virtual void addBox(std::vector<Object*> box) { m_box = box; }
        virtual void addItem(std::vector<Item*> item) { m_item = item; }    
        virtual Mix_Chunk* LoadSound(const std::string& path);
        virtual void Respawn();
        virtual int getTurn() { return turn_play; }
        virtual void addEffect(Effect* attack_effect) { m_Effect = attack_effect; }
        virtual void addEnemy(std::vector<Monster*> enemy) { m_enemy = enemy; }

        int attackStartTicks = 0;
    private :
        Animation* m_Animation;
        RigidBody* m_RigidBody;

        double m_JumpTime;
        double m_JumpForce;
        double m_AttackTime;
        double m_DeadTime;
        int turn_play;

        bool m_isJumping;
        bool m_isGrounded;
        bool m_isFalling;
        bool m_isRunning;
        bool m_isAttacking;
        bool m_dead;
        bool m_isPushing;

        Collider* m_Collider;
        Collider* m_Trap;
        std::vector<Object*> m_box;
        std::vector<Item*> m_item;
        std::vector<Monster*> m_enemy;

        Effect* m_Effect;

        Vector2D m_LastSafePosition;
        Vector2D m_lastSafePostion2;
        Mix_Chunk* m_getCoinSound = nullptr;
        Mix_Chunk* m_jumpSound = nullptr;
        Mix_Chunk* m_hurtSound = nullptr;
        Mix_Chunk* m_deadSound = nullptr;
};

#endif // SPRITES_H