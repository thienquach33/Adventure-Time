#ifndef OBJECT_H
#define OBJECT_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animation/Animation.h"
#include "../Physics/Collider.h"

#define JUMP_TIME 15.0f
#define JUMP_FORCE 12.0f

#define RUN_FORCE 4.0f
#define ATTACK_TIME 20.0f
#define PUSH_FORCE 2.0f

#define BACKWARD 1
#define FORWARD -1

class Object : public Character {
    public :    
        Object(Properties* props);
        virtual void Draw();
        virtual void Update(double dt);
        virtual void Clean();
        virtual void Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip = SDL_FLIP_NONE);
        virtual void SetAnimation(std::string animation_name, int num, int speed, int delay_attack = 0);
        virtual void AnimationState();
        virtual void setHit(bool m_hit) { m_isHitting = m_hit; }

        virtual void applydx(int dx) { this->dx = dx; }
        virtual void setType(int type) { m_type = type; }

        SDL_Rect getCollider() { return m_Collider->Get(); }

        int attackStartTicks = 0;

        bool isToBeDestroyed() const {
            return m_tobeDestroy;
        }
    private :
        Animation* m_Animation;
        RigidBody* m_RigidBody;

        float m_DestroyTimer = 0.0f;
        bool m_StartDestroyTimer = false;
        bool m_tobeDestroy = false;

        double m_JumpTime;
        double m_JumpForce;
        double m_AttackTime;

        int heal_of_box = 300;
        int m_type;
        int dx = 0;

        bool m_isJumping;
        bool m_isGrounded;
        bool m_isFalling;
        bool m_isRunning;
        bool m_isHitting;
        bool m_dead;

        Collider* m_Collider;

        Vector2D m_LastSafePosition;
};

#endif // OBJECT_H