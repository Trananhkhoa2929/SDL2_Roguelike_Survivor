// file: RangedAIComponent.h
#pragma once
#include "IComponent.h"

// Component mới này chứa dữ liệu cho AI có hành vi bắn xa.
class RangedAIComponent : public IComponent {
public:
    float attackRange;    // Khoảng cách tối đa để tấn công
    float attackCooldown; // Thời gian chờ giữa các lần bắn
    float timer;          // Bộ đếm thời gian cho cooldown

    RangedAIComponent(float range = 450.0f, float cooldown = 2.5f)
        : attackRange(range), attackCooldown(cooldown), timer(0.0f) {
    }
};