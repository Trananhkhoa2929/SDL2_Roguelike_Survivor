#pragma once
#include <SDL.h>

// MỚI: Hệ thống này không cần duyệt qua các thực thể.
// Nó chỉ nhận thời gian và renderer để thực hiện công việc.
class TimeRenderSystem {
public:
    // Nhận vào thời gian chơi (tính bằng giây) và vẽ nó lên màn hình.
    void Render(float gameTime, SDL_Renderer* renderer);
};