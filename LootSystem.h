#pragma once

class EntityManager;

// MỚI: Hệ thống này quản lý việc hút và nhặt các viên XP.
class LootSystem
{
public:
    void Update(EntityManager& entityManager, float deltaTime);
};