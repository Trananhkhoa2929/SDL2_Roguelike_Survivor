// src/Systems/WeaponControlSystem.h
#pragma once

class EntityManager;
class Entity;
// MỚI: Hệ thống này thay thế WeaponSystem cũ.
// Nó chịu trách nhiệm kích hoạt các loại vũ khí khác nhau.
class WeaponControlSystem {
public:
    void Update(EntityManager& entityManager, float deltaTime);
private:
    void UpdateGarlic(Entity* player, EntityManager& entityManager, float deltaTime);
    void UpdateWhip(Entity* player, EntityManager& entityManager, float deltaTime);
    // SỬA: Thêm khai báo cho hàm xử lý vũ khí mới
    void UpdateMagicWand(Entity* player, EntityManager& entityManager, float deltaTime);
};