#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector2D.h"

#define UNI_MASS 1.0f
#define GRAVITY 9.8f

class RigidBody {
    public : 
        RigidBody() {
            m_Mass = UNI_MASS;
            m_Gravity = GRAVITY;
        }

        inline void SetMass (double mass) { m_Mass = mass; }
        inline void SetGravity (double gravity) { m_Gravity = gravity; }
        inline void ApplyForce (Vector2D F) { m_Force = F; }
        inline void ApplyForceX (double FX) { m_Force.X = FX; }
        inline void ApplyForceY (double FY) { m_Force.Y = FY; }
        inline void UnSetForce () { m_Force = Vector2D(0, 0); }

        inline void ApplyFriction(Vector2D Fr) { m_Friction = Fr; }
        inline void UnSetFriction() { m_Friction = Vector2D(0, 0); }

        inline double getMass() { return m_Mass; }
        inline Vector2D Position() { return m_Position; }
        inline Vector2D Velocity() { return m_Velocity; }
        inline Vector2D Accelaration() { return m_Accelaration; }

        void Update(double dt) {
            m_Accelaration.X = (m_Force.X + m_Friction.X) / m_Mass;
            m_Accelaration.Y = m_Gravity + m_Force.Y / m_Mass;
            m_Velocity = m_Accelaration * dt;
            m_Position = m_Velocity * dt;
        }

    private : 
        double m_Mass;
        double m_Gravity;
        Vector2D m_Force;
        Vector2D m_Friction;

        Vector2D m_Position;
        Vector2D m_Velocity;
        Vector2D m_Accelaration;
};

#endif // RIGIDBODY_H