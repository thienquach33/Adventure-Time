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
        virtual void AnimationState(double dt);
        inline int getAttack() { return m_isAttacking; }
        virtual void addBox(std::vector<Object*> box) { m_box = box; }
        virtual void addItem(std::vector<Item*> item) { m_item = item; }    
        virtual Mix_Chunk* LoadSound(const std::string& path);
        virtual void Respawn();
        virtual int getTurn() { return turn_play; }
        virtual void addEffect(std::vector<Effect*> attack_effect) { m_Effect = attack_effect; }
        virtual void addEnemy(std::vector<Monster*> enemy) { m_enemy = enemy; }
        virtual void addHeal(std::vector<Item*> heal) { m_heal = heal; }    
        virtual void addGateLevel(Object* portal_gate) { m_portal_gate = portal_gate; }
        virtual void setType(int type) { m_type = type; }
        virtual int getLevel() { return level_cur; }
        virtual void setSpawnPosition(int x, int y) { m_Transform->X = x; m_Transform->Y = y; }
        virtual void addHealthBar(Item* health) { health_bar = health; }
        virtual void addFont(TTF_Font* font) { m_font = font; }
        virtual void addSword(Item* sword) { m_sword = sword; }
        virtual void addBomb(std::vector<Object*> bomb) { this->bomb = bomb; }
        virtual void addShip(Object* ship) { this->ship = ship; }
        virtual Vector2D getPosition() { return Vector2D(m_Transform->X, m_Transform->Y); }
        virtual void setPos() { m_Transform->X = 8 * 80, m_Transform->Y = 15 * 80; haveSword = false; }
        virtual void addBottle(std::vector<Item*> bottle) { m_bottle = bottle; }
        virtual void addMontestPortal(Decor* monster_portal) { this->monster_portal = monster_portal; }

        int attackStartTicks = 0;
    private :
        Animation* m_Animation;
        RigidBody* m_RigidBody;

        double m_JumpTime;
        double m_JumpForce;
        double m_AttackTime;
        double m_DeadTime;
        double m_nextLevelTime, m_recall = 0;
        int turn_play;

        bool haveSword = false;
        bool m_is_on_ship = false;
        bool m_isJumping;
        bool m_isGrounded;
        bool m_wasGrounded;
        bool m_isFalling;
        bool m_isRunning;
        bool m_dead;
        bool m_isPushing;
        bool m_nextLevel;
        int m_isAttacking = 0;

        int m_type;
        int level_cur = 1;

        Collider* m_Collider;
        Collider* m_Trap;
        std::vector<Object*> m_box;
        std::vector<Item*> m_item, m_heal, m_bottle;
        Item* health_bar;
        Item* m_sword;
        std::vector<Monster*> m_enemy, m_shotter;
        Object* m_portal_gate;

        std::vector<Effect*> m_Effect;
        Object* ship;
        Decor* monster_portal;

        Vector2D m_LastSafePosition;
        Vector2D m_lastSafePostion2;
        Mix_Chunk* m_getCoinSound = nullptr;
        Mix_Chunk* m_jumpSound = nullptr;
        Mix_Chunk* m_hurtSound = nullptr;
        Mix_Chunk* m_deadSound = nullptr;

        std::vector<Object*> bomb;

        TTF_Font* m_font;
};

#endif // SPRITES_H