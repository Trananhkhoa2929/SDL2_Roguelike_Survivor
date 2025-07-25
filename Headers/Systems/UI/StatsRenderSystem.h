#pragma once
#include <SDL.h>

class EntityManager;

// MỚI: Hệ thống này chịu trách nhiệm vẽ các chỉ số của người chơi lên màn hình.
class StatsRenderSystem {
public:
    void Render(EntityManager& entityManager, SDL_Renderer* renderer);
};