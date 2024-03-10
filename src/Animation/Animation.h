#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

class Animation {
    public : 
        Animation();
        void Update();
        void Draw(double x, double y, int w, int h, int sc);
        void setProps(std::string id, int num, int speed, SDL_RendererFlip flip = SDL_FLIP_NONE);
        int cur_frame;
    private :
        int num_frame;
        int animation_speed;
        std::string m_id, m_path;
        SDL_RendererFlip m_Flip;
};

#endif // ANIMATION_H