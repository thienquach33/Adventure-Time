#ifndef EFFECT_H
#define EFFECT_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animation/Animation.h"
#include "../Physics/Collider.h"

#define JUMP_TIME 15.0f

#define RUN_FORCE 4.0f
#define ATTACK_TIME 20.0f
#define PUSH_FORCE 2.0f

#define BACKWARD 1
#define FORWARD -1

class Effect : public Character {
    public :    
        Effect(Properties* props);
        virtual void Draw();
        virtual void Update(double dt);
        virtual void Clean();
        virtual void Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip = SDL_FLIP_NONE);
        virtual void SetAnimation(std::string animation_name, int x, int y, int num, int speed, int delay_attack, SDL_RendererFlip flip);   
        virtual void AnimationState();
        virtual void setAttack(bool attack) { m_isAttacking = attack; }
    private :
        Animation* m_Animation;

        bool m_isAttacking = false;

        Collider* m_Collider;
};

#endif // EFFECT_H