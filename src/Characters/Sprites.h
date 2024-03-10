#ifndef SPRITES_H
#define SPRITES_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animation/Animation.h"
#include "../Physics/Collider.h"

#define JUMP_TIME 15.0f
#define JUMP_FORCE 10.0f

class Sprites : public Character {
    public :    
        Sprites(Properties* props);
        virtual void Draw();
        virtual void Update(double dt);
        virtual void Clean();
        virtual void Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip = SDL_FLIP_NONE);
        virtual void SetAnimation(std::string animation_name, int num, int speed, SDL_RendererFlip slip = SDL_FLIP_NONE);
    private :
        Animation* m_Animation;
        RigidBody* m_RigidBody;

        double m_JumpTime;
        double m_JumpForce;

        Collider* m_Collider;

        Vector2D m_LastSafePosition;
};

#endif // SPRITES_H