
// File: src/Systems/GameEventSystem.h (MỚI)
#pragma once
#include <vector>
#include <string>

// Forward declare để tránh include vòng lặp
class EnemySpawnerSystem;

// MỚI: Các loại sự kiện có thể xảy ra
enum class GameEventType {
    CHANGE_SPAWN_INTERVAL, // Thay đổi tốc độ sinh sản
    ADD_ENEMY_TO_POOL,     // Thêm một loại kẻ thù mới
    SPAWN_BOSS             // (Cho tương lai) Tạo ra một con boss
};

// MỚI: Cấu trúc của một sự kiện
struct GameEvent {
    float triggerTime;      // Thời điểm sự kiện được kích hoạt (giây)
    GameEventType type;
    float numericValue;     // Giá trị số (ví dụ: tốc độ spawn mới)
    std::string stringValue; // Giá trị chuỗi (ví dụ: tên loại kẻ thù)
};

class GameEventSystem {
public:
    GameEventSystem();
    void Update(float gameTime, EnemySpawnerSystem& spawner);

private:
    std::vector<GameEvent> m_eventScript;
    size_t m_currentEventIndex; // Theo dõi sự kiện tiếp theo cần kiểm tra
};