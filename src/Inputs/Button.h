#ifndef BUTTON_H
#define BUTTON_H

#include "../Characters/Character.h"
#include "../Animation/Animation.h"
#include "../Physics/Collider.h"
#include "Mouse.h"

class Button : public Character {
    public :    
        Button(Properties* props);
        virtual void Draw();
        virtual void Update(double dt);
        virtual void Clean();
        virtual void Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip = SDL_FLIP_NONE);
        virtual void SetAnimation(std::string animation_name, int num, int speed, int delay_attack = 0);
        virtual void AnimationState();
        SDL_Rect getCollider() { return m_Collider->Get(); }

        bool isToBeDestroyed() const {
            return m_toBeDestroyed;
        }

        void setType(int type) { this->type = type; }
    private :
        int type;
        bool m_toBeDestroyed;
        Animation* m_Animation;
        Collider* m_Collider;
};

#endif // BUTTON_H