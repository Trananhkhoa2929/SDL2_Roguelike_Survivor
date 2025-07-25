#pragma once
#include <map>

class EntityManager;
class Map;
class Entity;

class TileEffectSystem {
public:
    void Update(EntityManager& entityManager, const Map* map, float deltaTime);

private:
    // Lưu trữ bộ đếm thời gian cho mỗi entity đứng trên dung nham
    // để đảm bảo sát thương được tính mỗi giây.
    std::map<Entity*, float> m_lavaTimers;
};