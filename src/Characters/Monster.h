#ifndef MONSTER_H
#define MONSTER_H

#include "Character.h"

class Monster : public Character {
    public :
        Monster(Properties* props);
        virtual void Draw();
        virtual void Update(double dt);
        virtual void Clean();
    private :
        int m_Frame, m_FrameCount, m_AnimationSpeed;
};

#endif // MONSTER_H