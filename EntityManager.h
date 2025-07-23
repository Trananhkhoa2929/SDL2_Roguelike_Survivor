#pragma once
#include <memory>
#include <vector>
#include "Entity.h"

class EntityManager
{
public:
	void Update(float deltaTime);
	void Render();
	void Refresh(); // loại bỏ entity bị hủy

	Entity& AddEntity();
	const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return m_Entities; } // <--- Thêm hàm này
private:
	std::vector<std::unique_ptr<Entity>> m_Entities;
	// NOTE: Hàng đợi này là một pattern rất tốt. Nó ngăn chặn việc thay đổi
	// vector m_Entities trong khi đang duyệt qua nó, tránh các lỗi
	// vô hiệu hóa iterator (iterator invalidation).

	std::vector<std::unique_ptr<Entity>> m_EntitiesToAdd; // <--- THÊM HÀNG ĐỢI NÀY

};