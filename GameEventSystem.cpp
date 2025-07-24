// File: src/Systems/GameEventSystem.cpp (MỚI)
#include "GameEventSystem.h"
#include "EnemySpawnerSystem.h"

GameEventSystem::GameEventSystem() : m_currentEventIndex(0) {
    // --- ĐÂY LÀ KỊCH BẢN CỦA MÀN CHƠI ---

    // 0-15 giây: Quái xuất hiện mỗi 2.5 giây
    m_eventScript.push_back({ 15.0f, GameEventType::CHANGE_SPAWN_INTERVAL, 2.0f, "" });

    // 30 giây: Brute bắt đầu xuất hiện
    m_eventScript.push_back({ 30.0f, GameEventType::ADD_ENEMY_TO_POOL, 0, "brute" });

    // 45 giây: Quái xuất hiện dồn dập hơn
    m_eventScript.push_back({ 45.0f, GameEventType::CHANGE_SPAWN_INTERVAL, 1.2f, "" });

    // 60 giây: Archer xuất hiện
    m_eventScript.push_back({ 60.0f, GameEventType::ADD_ENEMY_TO_POOL, 0, "archer" });

    // 90 giây (1:30): Nhịp độ tăng vọt
    m_eventScript.push_back({ 90.0f, GameEventType::CHANGE_SPAWN_INTERVAL, 0.7f, "" });



    // 180 giây (3:00): Chuẩn bị cho boss
    // m_eventScript.push_back({ 180.0f, GameEventType::SPAWN_BOSS, 0, "giant_brute" });
}

void GameEventSystem::Update(float gameTime, EnemySpawnerSystem& spawner) {
    // Chỉ kiểm tra nếu vẫn còn sự kiện trong kịch bản
    if (m_currentEventIndex >= m_eventScript.size()) {
        return;
    }

    // Lấy sự kiện tiếp theo
    const auto& nextEvent = m_eventScript[m_currentEventIndex];

    // Nếu thời gian game đã vượt qua thời điểm kích hoạt của sự kiện
    if (gameTime >= nextEvent.triggerTime) {
        // Thực thi sự kiện
        switch (nextEvent.type) {
        case GameEventType::CHANGE_SPAWN_INTERVAL:
            spawner.SetSpawnInterval(nextEvent.numericValue);
            break;
        case GameEventType::ADD_ENEMY_TO_POOL:
            spawner.AddEnemyTypeToPool(nextEvent.stringValue);
            break;
        case GameEventType::SPAWN_BOSS:
            // Logic tạo boss sẽ được thêm ở đây
            break;
        }

        // Chuyển sang sự kiện tiếp theo trong danh sách
        m_currentEventIndex++;
    }
}