// File: src/ECS/Components/DamageTextComponent.h (MỚI)
#pragma once
#include "IComponent.h"

// MỚI: Component này đánh dấu một thực thể là một hiệu ứng số sát thương nổi.
class DamageTextComponent : public IComponent {
public:
    float lifeTime;   // Tổng thời gian tồn tại (giây)
    float timer;      // Bộ đếm thời gian hiện tại
    float floatSpeed; // Tốc độ bay lên (pixel/giây)

    DamageTextComponent(float life = 0.6f, float speed = 50.0f)
        : lifeTime(life), timer(0.0f), floatSpeed(speed) {
    }
};