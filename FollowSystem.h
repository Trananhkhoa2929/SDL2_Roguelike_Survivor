#pragma once

class EntityManager;

// MỚI: Hệ thống này có nhiệm vụ cập nhật vị trí của các thực thể
// được đánh dấu là "đi theo" một thực thể khác.
class FollowSystem {
public:
    void Update(EntityManager& entityManager, float deltaTime);
};