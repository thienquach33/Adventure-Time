#include "Camera.h"

Camera* Camera::s_Instance = nullptr;

void Camera::Update(double dt) {
    if(m_Targets != nullptr) {
        m_ViewBox.x = m_Targets->x - (2960 / 2);
        m_ViewBox.y = m_Targets->y - (1760 / 2);

        if(m_ViewBox.x < 0) m_ViewBox.x = 0;
        if(m_ViewBox.y < 0) m_ViewBox.y = 0;

        if(m_ViewBox.x > (4 * 2960 - m_ViewBox.w)) {
            m_ViewBox.x = (4 * 2960 - m_ViewBox.w);
        }

        if(m_ViewBox.y > (1790 - m_ViewBox.h - 30)) {
            m_ViewBox.y = 1790 - m_ViewBox.h - 30;
        }

        m_Postision = Vector2D(m_ViewBox.x, m_ViewBox.y);
    }
} 