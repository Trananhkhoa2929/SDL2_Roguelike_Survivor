// src/Systems/EnemySpawnerSystem.h

#pragma once
#include <random>
#include <string>
#include <vector>

class EntityManager; // Dùng khai báo trước là đủ

class EnemySpawnerSystem
{
public:
    // Constructor với giá trị mặc định
    EnemySpawnerSystem(float interval = 2.0f);
    // Hàm cập nhật chính
    void Update(EntityManager& entityManager, float deltaTime);

    // MỚI: Các hàm public để điều khiển spawner
    void SetSpawnInterval(float newInterval);
    void AddEnemyTypeToPool(const std::string& enemyType);

private:
    void CreateEnemyByType(const std::string& type, EntityManager& entityManager);
    // Các hàm helper để tạo từng loại kẻ thù
    void CreateChaser(EntityManager& entityManager);
    void CreateBrute(EntityManager& entityManager);
    void CreateArcher(EntityManager& entityManager);
    // Hàm helper để lấy vị trí spawn ngẫu nhiên
    void GetRandomSpawnPosition(float& x, float& y);

    // Các biến thành viên
    float m_SpawnInterval;
    float m_SpawnTimer;

    // MỚI: Lưu danh sách các loại kẻ địch có thể xuất hiện
    std::vector<std::string> m_EnemySpawnPool;

    // Chỉ cần giữ lại bộ sinh số ngẫu nhiên.
    // Các bộ phân phối (distribution) có thể được tạo cục bộ khi cần.
    std::mt19937 m_RandomEngine;
};