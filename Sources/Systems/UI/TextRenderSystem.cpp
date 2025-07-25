// src/Systems/TextRenderSystem.cpp
#include "TextRenderSystem.h"
#include "TextComponent.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "AssetManager.h"
#include "Camera.h"
#include <SDL.h>


void TextRenderSystem::Render(EntityManager& entityManager, SDL_Renderer* renderer)
{
	SDL_Rect cameraBox = Camera::GetInstance()->GetViewBox();

	for (auto& entity : entityManager.GetEntities()) {
		if (entity->HasComponent<TextComponent>() && entity->HasComponent<TransformComponent>()) {
			auto& textComp = entity->GetComponent<TextComponent>();
			auto& transform = entity->GetComponent<TransformComponent>();

			// SỬA: Kiểm tra xem text có phải là UI cố định không
			float renderX = transform.position.x;
			float renderY = transform.position.y;

			if (!textComp.isFixedToScreen) {
				// Nếu là text trong thế giới game (như damage popup), trừ đi tọa độ camera
				renderX -= cameraBox.x;
				renderY -= cameraBox.y;
			}
			// REFACTOR: Lấy texture đã được cache từ AssetManager
			SDL_Texture* textTexture = AssetManager::GetInstance()->GetTextTexture(
				textComp.text,
				textComp.fontId,
				textComp.color
			);

			if (textTexture) {
				int textW, textH;
				SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
				SDL_Rect destRect = {
					static_cast<int>(renderX),
					static_cast<int>(renderY),
					textW,
					textH
				};
				SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
			}
		}
	}
}