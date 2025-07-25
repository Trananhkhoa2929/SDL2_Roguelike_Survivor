#pragma once
#include "IComponent.h"
#include "AssetManager.h"
#include <SDL.h>
#include <string>

class SpriteComponent : public IComponent
{
public:
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	// MỚI: Cờ để xác định sprite có bị ảnh hưởng bởi camera hay không.
	// Mặc định là false (là một phần của thế giới game).
	bool isFixedToScreen = false;

	SpriteComponent(const std::string& textureId, bool fixed = false) : isFixedToScreen(fixed)
	{
		m_Texture = AssetManager::GetInstance()->GetTexture(textureId);
		if (m_Texture) {
			SDL_QueryTexture(m_Texture, nullptr, nullptr, &srcRect.w, &srcRect.h);
		}
	}
	SpriteComponent(const std::string& textureId, int x, int y, int w, int h) {
		m_Texture = AssetManager::GetInstance()->GetTexture(textureId);
		srcRect = { x, y, w, h };
	}
	SDL_Texture* GetTexture() const { return m_Texture; }
	SDL_Rect GetSrcRect() const { return srcRect; }

private:
	SDL_Texture* m_Texture = nullptr;
	SDL_Rect srcRect = { 0, 0, 0, 0 };
};