// src/ECS/Components/WeaponComponents.h
#pragma once
#include "IComponent.h"

// MỚI: Component cho vũ khí Garlic (Tỏi)
class GarlicAuraComponent : public IComponent {
public:
    //int level = 1;
    float cooldown = 2.0f; // Thời gian giữa các lần gây sát thương
    float timer = 0.0f;
    float baseDamage = 5.0f;
    float baseRadius = 100.0f;
};

// MỚI: Component cho vũ khí Whip (Roi)
class WhipComponent : public IComponent {
public:
    //int level = 1;
    float cooldown = 1.5f;
    float timer = 0.0f;
    float baseDamage = 20.0f;
    float baseWidth = 150.0f;
    float baseHeight = 20.0f;
    int baseProjectileCount = 1; // Roi cấp cao có thể đánh ra sau

};

// MỚI: Component cho vũ khí Magic Wand (Quyền Trượng Phép Thuật)
class MagicWandComponent : public IComponent {
public:
    //int level = 1;
    float cooldown = 1.0f; // Thời gian hồi chiêu
    float timer = 0.0f;
    float baseDamage = 20.0f;
    float projectileSpeed = 300.0f;
    float maxRange = 800.0f;
};

// MỚI: Component tạm thời, đại diện cho một vùng gây sát thương
// Nó sẽ được thêm vào một thực thể tạm thời và bị xóa sau một khoảng ngắn.
class HitboxComponent : public IComponent {
public:
    float damage;
    float duration; // Tồn tại trong bao lâu (giây)
    float timer = 0.0f;
    // Có thể thêm các thuộc tính khác như knockback
    HitboxComponent(float dmg, float dur) : damage(dmg), duration(dur) {}
};
