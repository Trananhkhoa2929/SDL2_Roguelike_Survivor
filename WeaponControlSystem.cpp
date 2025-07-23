// src/Systems/WeaponControlSystem.cpp

#include "WeaponControlSystem.h"
#include "EntityManager.h"
#include "Entity.h"
#include "StatsComponent.h"
#include "WeaponComponents.h"
//#include "InputHandler.h"
//#include "Vector2D.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "PlayerControllerComponent.h"
//#include "AIComponent.h"

void WeaponControlSystem::Update(EntityManager& entityManager, float deltaTime) {
	Entity* player = nullptr;
	for (auto& entity : entityManager.GetEntities()) {
		if (entity->HasComponent<PlayerControllerComponent>()) {
			player = entity.get();
			break;
		}
	}
	if (!player) return;

	UpdateGarlic(player, entityManager, deltaTime);
	UpdateWhip(player, entityManager, deltaTime);
	UpdateMagicWand(player, entityManager, deltaTime);
	// ... gọi các hàm Update cho các vũ khí khác ở đây ...
}

// --- Thêm hàm mới để xử lý Magic Wand ---
void WeaponControlSystem::UpdateMagicWand(Entity* player, EntityManager& entityManager, float deltaTime) {
	if (!player->HasComponent<MagicWandComponent>() || !player->HasComponent<StatsComponent>()) return;

	auto& wand = player->GetComponent<MagicWandComponent>();
	auto& stats = player->GetComponent<StatsComponent>();

	wand.timer += deltaTime;
	float currentCooldown = wand.cooldown * stats.cooldownReduction;

	if (wand.timer < currentCooldown) {
		return;
	}
	wand.timer = 0.0f;

	// Logic tìm kẻ địch gần nhất (giữ nguyên)
	Entity* closestEnemy = nullptr;
	//float minDistanceSq = std::numeric_limits<float>::max();
	auto& playerTransform = player->GetComponent<TransformComponent>();

	// 1. Tính toán tầm bắn tối đa (bình phương) của vũ khí.
	float maxRangeSq = wand.maxRange * wand.maxRange;

	// 2. Khởi tạo khoảng cách nhỏ nhất bằng chính tầm bắn tối đa.
	// Điều này đảm bảo chúng ta chỉ tìm kiếm các mục tiêu trong phạm vi này.
	float minDistanceSq = maxRangeSq;

	for (auto& target : entityManager.GetEntities()) {
		if (target->IsActive() && target->HasComponent<ColliderComponent>() && target->GetComponent<ColliderComponent>().tag == "enemy") {
			auto& targetTransform = target->GetComponent<TransformComponent>();
			float distanceSq = playerTransform.position.distanceSq(targetTransform.position);
			// 3. Điều kiện bây giờ kiểm tra:
			//    a) Mục tiêu có gần hơn mục tiêu gần nhất trước đó không?
			//    b) Mục tiêu có nằm trong tầm bắn không (vì minDistanceSq được khởi tạo bằng maxRangeSq)
			if (distanceSq < minDistanceSq) {
				minDistanceSq = distanceSq;
				closestEnemy = target.get();
			}
		}
	}

	// Nếu tìm thấy mục tiêu, bắn!
	if (closestEnemy) {
		// SỬA LỖI 1 & 2: TÍNH TOÁN VỊ TRÍ TRUNG TÂM
		// Lấy lại logic tính toán chính xác từ hệ thống cũ của bạn.
		Vector2D playerCenter = playerTransform.position + Vector2D(playerTransform.width * playerTransform.scale / 2, playerTransform.height * playerTransform.scale / 2);

		auto& enemyTransform = closestEnemy->GetComponent<TransformComponent>();
		Vector2D enemyCenter = enemyTransform.position + Vector2D(enemyTransform.width * enemyTransform.scale / 2, enemyTransform.height * enemyTransform.scale / 2);

		Vector2D direction = enemyCenter - playerCenter;
		direction.normalize();
		// kết thúc tìm kiếm enemy 
		// SỬA: Đọc số lượng đạn từ StatsComponent
		int numProjectiles = 1 + stats.projectileCountBonus;

		for (int i = 0; i < numProjectiles; ++i) {
			Entity& bullet = entityManager.AddEntity();

			// SỬA: Khai báo bulletSize
			const int bulletSize = 16;

			// SỬA: Dùng đúng tên biến từ StatsComponent
			float currentDamage = wand.baseDamage * stats.damageMultiplier;
			float currentSpeed = wand.projectileSpeed * stats.projectileSpeedMultiplier;

			Vector2D spawnPos = playerCenter - Vector2D(bulletSize / 2.0f, bulletSize / 2.0f);

			bullet.AddComponent<TransformComponent>(spawnPos.x, spawnPos.y, bulletSize, bulletSize, 1.0);
			bullet.GetComponent<TransformComponent>().velocity = direction * currentSpeed;
			bullet.AddComponent<SpriteComponent>("bullet_sprite");
			bullet.AddComponent<ColliderComponent>("player_weapon");
			bullet.AddComponent<ProjectileComponent>(currentDamage, wand.maxRange, playerCenter);
			bullet.AddComponent<HitboxComponent>(currentDamage, 999.0f);
		}
	}
}

void WeaponControlSystem::UpdateGarlic(Entity* player, EntityManager& entityManager, float deltaTime) {
	if (!player->HasComponent<GarlicAuraComponent>() || !player->HasComponent<StatsComponent>()) return;

	auto& garlic = player->GetComponent<GarlicAuraComponent>();
	auto& stats = player->GetComponent<StatsComponent>();

	garlic.timer += deltaTime;
	// SỬA: Dùng đúng tên biến cooldownReduction
	float currentCooldown = garlic.cooldown * stats.cooldownReduction;

	if (garlic.timer >= currentCooldown) {
		garlic.timer = 0.0f;

		// Tạo một thực thể hitbox hào quang
		Entity& aura = entityManager.AddEntity();
		auto& playerTransform = player->GetComponent<TransformComponent>();
		float currentRadius = garlic.baseRadius * stats.areaSizeMultiplier;

		// Hitbox sẽ đi theo người chơi
		aura.AddComponent<TransformComponent>(playerTransform.position.x, playerTransform.position.y, currentRadius * 2, currentRadius * 2, 1.0);
		aura.GetComponent<TransformComponent>().entityToFollow = player; // Thêm thuộc tính này vào TransformComponent nếu cần

		// Thêm các component cần thiết cho hitbox
		aura.AddComponent<ColliderComponent>("player_weapon");
		float currentDamage = garlic.baseDamage * stats.damageMultiplier;
		aura.AddComponent<HitboxComponent>(currentDamage, 0.2f); // Tồn tại trong 0.2 giây
	}
}

void WeaponControlSystem::UpdateWhip(Entity* player, EntityManager& entityManager, float deltaTime) {
	if (!player->HasComponent<WhipComponent>() || !player->HasComponent<StatsComponent>()) return;

	auto& whip = player->GetComponent<WhipComponent>();
	auto& stats = player->GetComponent<StatsComponent>();

	whip.timer += deltaTime;
	float currentCooldown = whip.cooldown * stats.cooldownReduction;

	if (whip.timer >= currentCooldown) {
		whip.timer = 0.0f;

		auto& playerTransform = player->GetComponent<TransformComponent>();
		float currentWidth = whip.baseWidth * stats.areaSizeMultiplier;
		float currentHeight = whip.baseHeight;
		float currentDamage = whip.baseDamage * stats.damageMultiplier;

		// 1. TẠO HITBOX VÀ SPRITE CHO CÚ ĐÁNH PHÍA TRƯỚC
		Entity& whipHitboxFront = entityManager.AddEntity();
		float frontX = playerTransform.position.x + playerTransform.width;
		float frontY = playerTransform.position.y + (playerTransform.height / 3); // Điều chỉnh vị trí Y cho đẹp hơn

		whipHitboxFront.AddComponent<TransformComponent>(frontX, frontY, currentWidth, currentHeight, 1.0);
		whipHitboxFront.AddComponent<ColliderComponent>("player_weapon");
		whipHitboxFront.AddComponent<HitboxComponent>(currentDamage, 0.2f); // Tăng thời gian tồn tại một chút để dễ nhìn thấy

		// SỬA: Thêm SpriteComponent để hiển thị hình ảnh
		whipHitboxFront.AddComponent<SpriteComponent>("whip_sprite");


		// 2. TẠO HITBOX VÀ SPRITE CHO CÚ ĐÁNH PHÍA SAU (NẾU CÓ NÂNG CẤP)
		// Logic này sẽ hoạt động khi bạn có nâng cấp "+1 Amount"
		if (stats.projectileCountBonus > 0) {
			Entity& whipHitboxBack = entityManager.AddEntity();
			float backX = playerTransform.position.x - currentWidth; // Vị trí ở bên trái người chơi

			whipHitboxBack.AddComponent<TransformComponent>(backX, frontY, currentWidth, currentHeight, 1.0);
			whipHitboxBack.AddComponent<ColliderComponent>("player_weapon");
			whipHitboxBack.AddComponent<HitboxComponent>(currentDamage, 0.2f);

			// SỬA: Thêm SpriteComponent và lật ngược hình ảnh
			auto& backSprite = whipHitboxBack.AddComponent<SpriteComponent>("whip_sprite");
			backSprite.flip = SDL_FLIP_HORIZONTAL; // Lật ngược sprite theo chiều ngang
		}
	}
}

