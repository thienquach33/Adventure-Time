#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

class Animation {
    public : 
        Animation();
        void Update();
        void Draw(double x, double y, int w, int h, int sc, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void setProps(std::string id, int num, int speed);
        int cur_frame;
    private :
        int num_frame;
        int animation_speed;
        std::string m_id, m_path;
};

#endif // ANIMATION_H