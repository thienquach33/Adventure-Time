#ifndef ITEM_H
#define ITEM_H

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

class Item : public Character {
    public :    
        Item(Properties* props);
        virtual void Draw();
        virtual void Update(double dt);
        virtual void Clean();
        virtual void Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip = SDL_FLIP_NONE);
        virtual void SetAnimation(std::string animation_name, int num, int speed, int delay_attack = 0);
        virtual void AnimationState(double dt);
        virtual void setHit(bool m_hit) { m_isHitting = m_hit; }

        virtual void applydx(int dx) { this->dx = dx; }
        virtual void eat() { m_isEtten = true; }
        virtual void setType(int type) { m_type = type; }
        inline int getType() { return m_type; }
        virtual void setCollider(int x, int y, int w, int h) { m_Collider->Set(x, y, w, h); }
        void setPos(int x, int y) {
            m_Transform->X = x;
            m_Transform->Y = y;
        }

        SDL_Rect getCollider() { return m_Collider->Get(); }

        int attackStartTicks = 0;

        void addMap(Item* map) {
            this->map = map;
        }

        void setShow(bool show) {
            isShow = show;
        }

        bool getShow() {
            return isShow;
        }

        bool isToBeDestroyed() const {
            return m_tobeDestroy;
        }

        void haveMapItem() {
            haveMap = 1;
        }

        bool getHaveMap() {
            return haveMap;
        }
    private :
        Animation* m_Animation;
        RigidBody* m_RigidBody;

        double m_DestroyTimer = 0.0f;
        double m_EatTimer = 0.0f;
        bool m_StartDestroyTimer = false;
        bool m_tobeDestroy = false;
        bool add_point = false;

        double m_JumpTime;
        double m_JumpForce;
        double m_AttackTime;

        bool isShow = 0;

        int heal_of_box = 100;
        int m_type;

        int dx = 0;

        bool unlock4 = 0;
        bool m_isJumping;
        bool m_isGrounded;
        bool m_isFalling;
        bool m_isRunning;
        bool m_isHitting;
        bool m_dead;
        bool m_isEtten;
        bool haveMap = 0;

        Collider* m_Collider;
        Item* map;

        Vector2D m_LastSafePosition;
};

#endif // ITEM_H