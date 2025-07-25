// src/Systems/EnemySpawnerSystem.cpp

#include "EnemySpawnerSystem.h"
#include "EntityManager.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "AIComponent.h"
#include "HealthComponent.h"
#include "LootDropComponent.h"
#include "ScoreValueComponent.h"
#include "RangedAIComponent.h"
#include "Camera.h"
#include <chrono>
#include <iostream> // <--- Thêm vào để debug

EnemySpawnerSystem::EnemySpawnerSystem(float interval)
    : m_SpawnInterval(interval), m_SpawnTimer(0.0f)
{
    m_RandomEngine.seed(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    // Khởi đầu chỉ cho phép sinh ra Chaser
    m_EnemySpawnPool.push_back("chaser");
}

// MỚI: Các hàm điều khiển
void EnemySpawnerSystem::SetSpawnInterval(float newInterval) {
    m_SpawnInterval = newInterval;
    std::cout << "[EVENT] Toc do sinh quai da thay doi thanh: " << newInterval << "s" << std::endl;
}

void EnemySpawnerSystem::AddEnemyTypeToPool(const std::string& enemyType) {
    // Tránh thêm trùng lặp
    for (const auto& type : m_EnemySpawnPool) {
        if (type == enemyType) return;
    }
    m_EnemySpawnPool.push_back(enemyType);
    std::cout << "[EVENT] Loai quai moi da xuat hien: " << enemyType << "!" << std::endl;
}

void EnemySpawnerSystem::Update(EntityManager& entityManager, float deltaTime) {
    // --- DEBUG ---: In ra để xem hàm Update có được gọi và delta time có hợp lệ không
    // std::cout << "EnemySpawnerSystem Update - DeltaTime: " << deltaTime << " | Timer: " << m_SpawnTimer << std::endl;

    m_SpawnTimer += deltaTime;

    if (m_SpawnTimer >= m_SpawnInterval)
    {
        // --- DEBUG ---: In ra khi điều kiện spawn được đáp ứng
        std::cout << "--- SPAWNING ENEMY! ---" << std::endl;

        m_SpawnTimer = 0.0f;

        if (m_EnemySpawnPool.empty()) return;

        // Chọn ngẫu nhiên một loại kẻ thù từ pool
        std::uniform_int_distribution<int> dist(0, m_EnemySpawnPool.size() - 1);
        std::string enemyToSpawn = m_EnemySpawnPool[dist(m_RandomEngine)];

        // Tạo kẻ thù dựa trên loại đã chọn
        CreateEnemyByType(enemyToSpawn, entityManager);
    }
}

// MỚI: Hàm trung gian để gọi đúng hàm tạo kẻ thù
void EnemySpawnerSystem::CreateEnemyByType(const std::string& type, EntityManager& entityManager) {
    if (type == "chaser") {
        CreateChaser(entityManager);
    }
    else if (type == "brute") {
        CreateBrute(entityManager);
    }
    else if (type == "archer") {
        CreateArcher(entityManager);
    }
    // Thêm các loại kẻ thù khác ở đây
}

void EnemySpawnerSystem::GetRandomSpawnPosition(float& x, float& y)
{
    // SỬA: Bây giờ sẽ sinh ra quái xung quanh camera, không phải màn hình
    SDL_Rect cameraBox = Camera::GetInstance()->GetViewBox();
    int spawnPadding = 50; // Khoảng cách ngoài màn hình để sinh quái

    std::uniform_int_distribution<int> edgeDist(0, 3);
    int edge = edgeDist(m_RandomEngine);

    // Sinh ngẫu nhiên trên các cạnh của một hình chữ nhật lớn hơn camera
    switch (edge) {
    case 0: // Top edge
        x = std::uniform_real_distribution<float>(cameraBox.x - spawnPadding, cameraBox.x + cameraBox.w + spawnPadding)(m_RandomEngine);
        y = cameraBox.y - spawnPadding;
        break;
    case 1: // Bottom edge
        x = std::uniform_real_distribution<float>(cameraBox.x - spawnPadding, cameraBox.x + cameraBox.w + spawnPadding)(m_RandomEngine);
        y = cameraBox.y + cameraBox.h + spawnPadding;
        break;
    case 2: // Left edge
        x = cameraBox.x - spawnPadding;
        y = std::uniform_real_distribution<float>(cameraBox.y - spawnPadding, cameraBox.y + cameraBox.h + spawnPadding)(m_RandomEngine);
        break;
    case 3: // Right edge
        x = cameraBox.x + cameraBox.w + spawnPadding;
        y = std::uniform_real_distribution<float>(cameraBox.y - spawnPadding, cameraBox.y + cameraBox.h + spawnPadding)(m_RandomEngine);
        break;
    }
}

void EnemySpawnerSystem::CreateChaser(EntityManager& entityManager) {
    Entity& chaser = entityManager.AddEntity();
    float spawnX, spawnY;
    GetRandomSpawnPosition(spawnX, spawnY);

    chaser.AddComponent<TransformComponent>(spawnX, spawnY, 32, 32, 1.5);
    chaser.AddComponent<SpriteComponent>("chaser_sprite");
    chaser.AddComponent<ColliderComponent>("enemy");
    chaser.AddComponent<AIComponent>();
    chaser.AddComponent<HealthComponent>(50);
    chaser.AddComponent<LootDropComponent>(25, 5); // Chaser rơi ra 25 XP, 5 gold
    chaser.AddComponent<ScoreValueComponent>(10);
  
    // --- DEBUG ---
    std::cout << "Created Chaser at (" << spawnX << ", " << spawnY << ")" << std::endl;
}

void EnemySpawnerSystem::CreateBrute(EntityManager& entityManager) {
    Entity& brute = entityManager.AddEntity();
    float spawnX, spawnY;
    GetRandomSpawnPosition(spawnX, spawnY);

    brute.AddComponent<TransformComponent>(spawnX, spawnY, 48, 48, 2.0);
    brute.AddComponent<SpriteComponent>("brute_sprite");
    brute.AddComponent<ColliderComponent>("enemy");
    brute.AddComponent<AIComponent>();
    brute.AddComponent<HealthComponent>(150);
    brute.AddComponent<LootDropComponent>(75, 15); // Brute rơi ra 75 XP
    brute.AddComponent<ScoreValueComponent>(30);
    // --- DEBUG ---
    std::cout << "Created Brute at (" << spawnX << ", " << spawnY << ")" << std::endl;
}

// Thêm hàm này vào EnemySpawnerSystem.cpp
void EnemySpawnerSystem::CreateArcher(EntityManager& entityManager) {
    Entity& archer = entityManager.AddEntity();
    float spawnX, spawnY;
    GetRandomSpawnPosition(spawnX, spawnY);

    archer.AddComponent<TransformComponent>(spawnX, spawnY, 32, 32, 1.5);
    archer.AddComponent<SpriteComponent>("archer_sprite"); // Cần texture mới
    archer.AddComponent<ColliderComponent>("enemy");
    archer.AddComponent<HealthComponent>(30); // Máu giấy hơn
    archer.AddComponent<LootDropComponent>(35, 8);
    archer.AddComponent<ScoreValueComponent>(15);
    archer.AddComponent<RangedAIComponent>(500.0f, 2.0f); // Tầm bắn 500px, hồi chiêu 3 giây
}