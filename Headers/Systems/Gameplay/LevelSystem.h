#pragma once
class EntityManager;
class PlayState;

// MỚI: Hệ thống này kiểm tra và xử lý việc lên cấp.
class LevelSystem
{
public:
    // SỬA: Cần một con trỏ tới PlayState
    void Update(EntityManager& entityManager, PlayState* playState, float deltaTime);
};