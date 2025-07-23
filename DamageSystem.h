#pragma once

class EntityManager;

// MỚI: Hệ thống này chỉ có một nhiệm vụ: xử lý va chạm giữa hitbox vũ khí và kẻ thù.
class DamageSystem {
public:
    void Update(EntityManager& entityManager, float deltaTime);
};