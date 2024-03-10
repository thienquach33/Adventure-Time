#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include "../Physics/Point.h"
#include "../Physics/Vector2D.h"

class Camera {
    public :    
        void Update(double dt); 
        inline static Camera* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new Camera(); }
        inline SDL_Rect GetViewBox() { return m_ViewBox; }
        inline Vector2D GetPostision() { return m_Postision; }
        inline void SetTarget(Point* target) { m_Targets = target; }

    private :
        Camera() {
            m_ViewBox = {0, 0, 2960, 1760};
        }

        Point* m_Targets;
        Vector2D m_Postision;
        SDL_Rect m_ViewBox;
        static Camera* s_Instance;

};

#endif // CAMERA_H