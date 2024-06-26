#ifndef DECOR_H
#define DECOR_H

#include "Character.h"
#include "../Physics/RigidBody.h"
#include "../Animation/Animation.h"
#include "../Physics/Collider.h"
#include "Monster.h"

#define JUMP_TIME 15.0f
#define JUMP_FORCE 12.0f

#define RUN_FORCE 4.0f
#define ATTACK_TIME 20.0f
#define PUSH_FORCE 2.0f

#define BACKWARD 1
#define FORWARD -1

class Decor : public Character {
    public :    
        Decor(Properties* props);
        virtual void Draw();
        virtual void Update(double dt);
        virtual void Clean();
        virtual void Load(std::string name_animation, std::string path_animation, int num, SDL_RendererFlip flip = SDL_FLIP_NONE);
        virtual void SetAnimation(std::string animation_name, int num, int speed, int delay_attack = 0);
        virtual void AnimationState(double dt);

        virtual void setType(int type) { m_type = type; }
        virtual void applydx(int dx) { this->dx = dx; }
        virtual void setOut(bool out) { m_out = out; }
        inline bool getVar() { return var; }
        virtual void setNearPlayer(bool near) { near_player = near; }

        inline bool getSpawnEnemy() { return spawn_enemy; }

        SDL_Rect getCollider() { return m_Collider->Get(); }

        std::vector<Monster*> getEnemy() { return m_enemy; }

        int attackStartTicks = 0;
    private :
        bool m_out = false;
        double m_TimeVar = 0;
        bool m_isTurning = false;
        bool var = false;
        double helm_time_idle = 0;
        double helm_time_turn = 0;
        double m_time_appear = 0;
        double m_time_idle = 0;
        double m_time_disappear = 0;
        double m_respawn = 0;
        double delay_time = 0;
        bool portal_show = 1;
        int m_type;
        int dx = 0;
        bool m_portal_idle = 0;
        bool m_portal_appear = true;
        bool m_portal_disappear = 0;
        bool near_player = 0;
        bool spawn_enemy = 0;
        int num_enemy = 0;
        Animation* m_Animation;
        RigidBody* m_RigidBody;
        Collider* m_Collider;
        Vector2D m_LastSafePosition;

        std::vector<Monster*> m_enemy;
};

#endif // DECOR_H