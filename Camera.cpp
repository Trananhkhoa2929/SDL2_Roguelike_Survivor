// File: src/Camera.cpp (MỚI)
#include "Camera.h"

Camera* Camera::s_Instance = nullptr;

void Camera::Update(Vector2D targetPos, int worldWidth, int worldHeight) {
    // Đặt tâm của camera vào vị trí của mục tiêu
    m_viewBox.x = static_cast<int>(targetPos.x - SCREEN_WIDTH / 2);
    m_viewBox.y = static_cast<int>(targetPos.y - SCREEN_HEIGHT / 2);

    // Giới hạn camera không đi ra ngoài biên của thế giới
    if (m_viewBox.x < 0) {
        m_viewBox.x = 0;
    }
    if (m_viewBox.y < 0) {
        m_viewBox.y = 0;
    }
    if (m_viewBox.x > worldWidth - m_viewBox.w) {
        m_viewBox.x = worldWidth - m_viewBox.w;
    }
    if (m_viewBox.y > worldHeight - m_viewBox.h) {
        m_viewBox.y = worldHeight - m_viewBox.h;
    }
}