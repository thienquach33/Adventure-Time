#ifndef SPRITES_H
#define SPRITES_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animation/Animation.h"
#include "../Physics/Collider.h"

#define JUMP_TIME 15.0f
#define JUMP_FORCE 12.0f

#define RUN_FORCE 4.0f
#define ATTACK_TIME 20.0f

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

        int attackStartTicks = 0;
    private :
        Animation* m_Animation;
        RigidBody* m_RigidBody;

        double m_JumpTime;
        double m_JumpForce;
        double m_AttackTime;

        bool m_isJumping;
        bool m_isGrounded;
        bool m_isFalling;
        bool m_isRunning;
        bool m_isAttacking;

        Collider* m_Collider;

        Vector2D m_LastSafePosition;
};

#endif // SPRITES_H