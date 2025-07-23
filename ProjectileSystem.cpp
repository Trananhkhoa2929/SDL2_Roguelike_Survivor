// src/Systems/ProjectileSystem.cpp

#include "ProjectileSystem.h"
#include "EntityManager.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "ProjectileComponent.h"

void ProjectileSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		// Chỉ xử lý các thực thể là đạn và đang hoạt động
		if (entity->IsActive() && entity->HasComponent<ProjectileComponent>())
		{
			auto& transform = entity->GetComponent<TransformComponent>();
			auto& projectile = entity->GetComponent<ProjectileComponent>();

			// PERFORMANCE: So sánh bình phương khoảng cách để tránh dùng sqrt().
			float distanceSq = transform.position.distanceSq(projectile.startPosition);
			float maxRangeSq = projectile.maxRange * projectile.maxRange;

			if (distanceSq >= maxRangeSq)
			{
				entity->Destroy(); // Đánh dấu để hủy viên đạn
			}
		}
	}
}