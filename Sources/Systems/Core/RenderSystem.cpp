#include "RenderSystem.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "Camera.h"
#include <SDL.h>

void RenderSystem::Render(EntityManager& entityManager, SDL_Renderer* renderer)
{
    SDL_Rect cameraBox = Camera::GetInstance()->GetViewBox();

    for (auto& entity : entityManager.GetEntities())
    {
        if (entity->HasComponent<TransformComponent>() && entity->HasComponent<SpriteComponent>())
        {
            auto& transform = entity->GetComponent<TransformComponent>();
            auto& sprite = entity->GetComponent<SpriteComponent>();

            // KIỂM TRA QUAN TRỌNG NẰM Ở ĐÂY
            int renderX, renderY;

            if (sprite.isFixedToScreen) {
                // Nếu là UI, vẽ trực tiếp tại tọa độ của nó trên màn hình
                renderX = static_cast<int>(transform.position.x);
                renderY = static_cast<int>(transform.position.y);
            }
            else {
                // Nếu là vật thể trong game, trừ đi tọa độ camera
                renderX = static_cast<int>(transform.position.x - cameraBox.x);
                renderY = static_cast<int>(transform.position.y - cameraBox.y);
            }

            SDL_Rect src = sprite.GetSrcRect();
            SDL_Rect dest = {
                renderX,
                renderY,
                static_cast<int>(transform.width * transform.scale),
                static_cast<int>(transform.height * transform.scale)
            };

            // Tối ưu hóa (Culling)
            if (dest.x + dest.w > 0 && dest.x < SCREEN_WIDTH &&
                dest.y + dest.h > 0 && dest.y < SCREEN_HEIGHT)
            {
                SDL_RenderCopyEx(renderer, sprite.GetTexture(), &src, &dest, transform.rotation, nullptr, sprite.flip);
            }
        }
    }
}

