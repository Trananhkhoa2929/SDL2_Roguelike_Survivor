// src/ECS/Components/ColliderComponent.cpp

#include "ColliderComponent.h"
#include "Entity.h" // <--- QUAN TRỌNG: Include đầy đủ Entity.h ở đây
#include "TransformComponent.h"

// Định nghĩa constructor
ColliderComponent::ColliderComponent(const std::string& tag) : tag(tag) {}

// Định nghĩa hàm Init
void ColliderComponent::Init()
{
	// Luôn kiểm tra sự tồn tại của component phụ thuộc trước khi sử dụng.

	if (!entity->HasComponent<TransformComponent>()) {
		return;
	}

	auto& transform = entity->GetComponent<TransformComponent>();
	hitbox = {
		static_cast<int>(transform.position.x),
		static_cast<int>(transform.position.y),
		// PERFORMANCE: Tính toán kích thước hitbox (phụ thuộc vào scale) một lần ở đây.
		// Việc này hiệu quả hơn là tính toán lại mỗi frame trong Update,
		// vì scale thường không thay đổi.

		static_cast<int>(transform.width * transform.scale),
		static_cast<int>(transform.height * transform.scale)
	};
}

// Định nghĩa hàm Update
void ColliderComponent::Update(float deltaTime)
{
	// Trong Update, chúng ta chỉ cần cập nhật vị trí của hitbox
	// để khớp với vị trí của TransformComponent.

	auto& transform = entity->GetComponent<TransformComponent>();
	hitbox.x = static_cast<int>(transform.position.x);
	hitbox.y = static_cast<int>(transform.position.y);
	// REFACTOR: Nếu game của bạn cho phép thay đổi scale trong lúc chạy,
		// bạn sẽ cần một cơ chế để cập nhật lại hitbox.w và hitbox.h.
		// Ví dụ, có thể thêm một hàm như `UpdateHitboxScale()` và gọi nó
		// khi scale của TransformComponent thay đổi.
		// hitbox.w = static_cast<int>(transform.width * transform.scale);
		// hitbox.h = static_cast<int>(transform.height * transform.scale);

}