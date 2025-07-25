#pragma once
#include <SDL.h>
#include "Vector2D.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

class Camera {
public:
    static Camera* GetInstance() {
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Camera();
    }

    // Cập nhật vị trí của camera để nó bám theo mục tiêu (người chơi)
    void Update(Vector2D targetPos, int worldWidth, int worldHeight);

    // Lấy ra hình chữ nhật đại diện cho khung nhìn của camera
    const SDL_Rect& GetViewBox() const { return m_viewBox; }

private:
    Camera() : m_viewBox({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }) {}

    SDL_Rect m_viewBox;
    static Camera* s_Instance;
};