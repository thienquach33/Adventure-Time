#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "IObject.h"
#include "../Physics/Transform.h"
#include <SDL2/SDL.h>
#include "../Physics/Point.h"

struct Properties {
    public :
        Properties(std::string textureID, int x, int y, int width, int height, double sc, SDL_RendererFlip flip = SDL_FLIP_NONE) {
            X = x;
            Y = y;
            Flip = flip;
            Width = width;
            Height = height;
            TextureID = textureID;
            scale = sc;
        }
    public :
        std::string TextureID, path;
        int Width, Height;
        double X, Y, scale;
        SDL_RendererFlip Flip;
};

class GameObject : public IObject{
    public :
        GameObject(Properties* props) : m_TextureID(props->TextureID),
        m_Width(props->Width), m_Height(props->Height), m_Flip(props->Flip), m_scale(props->scale) {
            m_Transform = new Transform(props->X, props->Y);
            double px = props->X + props->Width / 2;
            double py = props->Y + props->Height / 2;
            
            m_Origin =  new Point(px, py);
        }

        inline Point* GetOrigin() { return m_Origin; }

        virtual void Draw() = 0;
        virtual void Clean() = 0;
        virtual void Update(double dt) = 0;

    protected : 
        Point* m_Origin;
        Transform* m_Transform;
        int m_Width, m_Height;
        double m_scale;
        std::string m_TextureID;
        SDL_RendererFlip m_Flip;
};


#endif // GAMEOBJECT_H