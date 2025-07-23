#pragma once
#include <vector>
#include <memory>

class EntityManager;
struct SDL_Renderer;

class RenderSystem
{
public:
	void Render(EntityManager& entityManager, SDL_Renderer* renderer);
};