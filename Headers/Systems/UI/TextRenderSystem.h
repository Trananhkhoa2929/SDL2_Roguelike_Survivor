#pragma once

class EntityManager;
struct SDL_Renderer;

class TextRenderSystem
{
public:
	void Render(EntityManager& entityManager, SDL_Renderer* renderer);
};