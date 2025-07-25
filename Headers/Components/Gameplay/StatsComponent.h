#pragma once
#include "IComponent.h"

// MỚI: Component này chứa tất cả các chỉ số có thể được nâng cấp.
class StatsComponent : public IComponent {
public:
    // Chỉ số cơ bản (có thể không đổi)
    float baseMoveSpeed = 300.0f;
    int baseMaxHealth = 100;

    // --- Các chỉ số có thể nâng cấp ---
    // Các hệ số nhân (bắt đầu từ 1.0 = 100%)
    float moveSpeedMultiplier = 1.0f;
    float maxHealthMultiplier = 1.0f;
    float damageMultiplier = 1.0f;
    float cooldownReduction = 1.0f;      // Thời gian hồi chiêu, < 1.0 là nhanh hơn
    float areaSizeMultiplier = 1.0f;     // Kích thước/vùng ảnh hưởng
    float projectileSpeedMultiplier = 1.0f;
    float pickupRangeMultiplier = 1.0f;

    // Các giá trị cộng thêm (bắt đầu từ 0)
    int projectileCountBonus = 0; // Số lượng đạn/đòn tấn công thêm

    // Hàm tiện ích để lấy giá trị cuối cùng, đã áp dụng các hệ số
    float GetCurrentMoveSpeed() const { return baseMoveSpeed * moveSpeedMultiplier; }
    int GetCurrentMaxHealth() const { return static_cast<int>(baseMaxHealth * maxHealthMultiplier); }
};