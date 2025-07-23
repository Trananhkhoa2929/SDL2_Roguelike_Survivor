#pragma once
#include <SDL.h>

class EntityManager;

class CollisionSystem
{
public:
	void Update(EntityManager& entityManager);

private:
	bool CheckAABBCollision(const SDL_Rect& a, const SDL_Rect& b);
};