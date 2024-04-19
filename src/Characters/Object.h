#ifndef OBJECT_H
#define OBJECT_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animation/Animation.h"
#include "../Physics/Collider.h"
#include "Decor.h"
#include "item.h"

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
        virtual void AnimationState(double dt);
        virtual void setHit(bool m_hit) { m_isHitting = m_hit; }

        virtual void applydx(int dx) { this->dx = dx; }
        virtual void setType(int type) { m_type = type; }
        void setDestroy(bool destroy) { m_tobeDestroy = destroy; }
        void respawn(double x, double y) { m_Transform->X = x; m_Transform->Y = y; }
        void addExploration(Object* explore) { this->explore = explore; }
        void setExploring(bool exploring) { m_exploring = exploring; }
        bool getExploring() { return m_exploring; }
        bool isFalling() { return m_isFalling; }
        void active(bool uuui) { active_bomb = uuui; }
        Object* getExploration() { return explore; }
        void addSail(Decor* sail) { this->sail = sail; }
        Decor* getSail() { return sail; }
        SDL_Rect getCollider() { return m_Collider->Get(); }
        inline int getDx() { return dx; }
        bool moving() {
            return (!sail->getVar() && dx != 0);
        }

        int attackStartTicks = 0;

        bool isToBeDestroyed() const {
            return m_tobeDestroy;
        }

        void setOldPos(std::pair<int, int> pos) { old_pos = pos; }
        void addCoin(Item* new_coin) { this->coin = coin; }
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
        bool m_exploring = false;
        bool m_dead;
        bool active_bomb = false;
        double m_explore_time = 0;

        Collider* m_Collider;

        Object* explore;
        Decor* sail;

        int now = 1;

        Vector2D m_LastSafePosition;
        std::pair<int, int> old_pos;

        Item* coin;
};

#endif // OBJECT_H