#include "PlayerInputSystem.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "InputHandler.h"
#include "StatsComponent.h"
#include "PlayState.h"

//const int WORLD_WIDTH = 3000;
//const int WORLD_HEIGHT = 3000;

void PlayerInputSystem::Update(EntityManager& entityManager, float deltaTime)
{
	auto input = InputHandler::GetInstance();
	for (auto& entity : entityManager.GetEntities())
	{
		if (entity->HasComponent<PlayerControllerComponent>() && entity->HasComponent<TransformComponent>())
		{
			auto& transform = entity->GetComponent<TransformComponent>();
			// SỬA: Lấy tốc độ từ StatsComponent thay vì giá trị cứng
			float currentSpeed = m_Speed; // Giữ lại tốc độ mặc định
			if (entity->HasComponent<StatsComponent>()) {
             currentSpeed = entity->GetComponent<StatsComponent>().GetCurrentMoveSpeed();
			}

			transform.velocity = { 0.f,0.f };
			
			if (input->IsKeyDown(SDL_SCANCODE_W) || input->IsKeyDown(SDL_SCANCODE_UP)) {
				transform.velocity.y = -1;
			}
			if (input->IsKeyDown(SDL_SCANCODE_S) || input->IsKeyDown(SDL_SCANCODE_DOWN)) {
				transform.velocity.y = 1;
			}
			if (input->IsKeyDown(SDL_SCANCODE_D) || input->IsKeyDown(SDL_SCANCODE_RIGHT)) {
				transform.velocity.x = 1;
			}
			if (input->IsKeyDown(SDL_SCANCODE_A) || input->IsKeyDown(SDL_SCANCODE_LEFT)) {
				transform.velocity.x = -1;
			}
			if (transform.velocity.length() > 0) {
				transform.velocity.normalize();
			}
			transform.velocity = transform.velocity * currentSpeed;
			if (transform.position.x < 0) transform.position.x = 0;
			if (transform.position.y < 0) transform.position.y = 0;
			if (transform.position.x > WORLD_WIDTH - transform.width) transform.position.x = WORLD_WIDTH - transform.width;
			if (transform.position.y > WORLD_HEIGHT - transform.height) transform.position.y = WORLD_HEIGHT - transform.height;
		}
	}
}