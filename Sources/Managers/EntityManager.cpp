#include "EntityManager.h"
#include <algorithm>

void EntityManager::Update(float deltaTime) 
{
    // NOTE: Cập nhật các thực thể đang chờ được thêm vào.
    // Điều này đảm bảo các thực thể mới tạo cũng được cập nhật trong cùng frame.
    for (auto& e : m_EntitiesToAdd) {
        e->Update(deltaTime);
    }

	for (auto& e : m_Entities) {
		e->Update(deltaTime);
	}
}

void EntityManager::Render()
{
	for (auto& e : m_Entities) {
		e->Render();
	}
}

void EntityManager::Refresh() 
{
    // 1. Xóa tất cả các thực thể đã bị đánh dấu là không hoạt động (m_IsActive == false).
    // Sử dụng remove_if-erase idiom là cách làm chuẩn và hiệu quả trong C++.

    m_Entities.erase(
        std::remove_if(m_Entities.begin(), m_Entities.end(), [](const std::unique_ptr<Entity>& entity) {
            return !entity->IsActive();
            }),
        m_Entities.end()
    );

    // 2. Thêm các thực thể mới từ hàng đợi vào danh sách chính.
        // Phải thực hiện bước này SAU KHI xóa để tránh các vấn đề logic phức tạp.
        // Việc thêm vào cuối vector trong khi đang duyệt (nếu có) sẽ không làm
        // vô hiệu hóa các iterator hiện có.
    for (auto& newEntity : m_EntitiesToAdd) {
        m_Entities.emplace_back(std::move(newEntity));
    }

    // 3. Xóa sạch hàng đợi
    m_EntitiesToAdd.clear();
}

Entity& EntityManager::AddEntity() 
{
    Entity* e = new Entity();
    std::unique_ptr<Entity> uPtr{ e };

    // Thay vì thêm trực tiếp, ta đưa vào hàng đợi
    m_EntitiesToAdd.emplace_back(std::move(uPtr));
    // WARNING: Trả về một tham chiếu (Entity&) đến một đối tượng có vòng đời
    // được quản lý bởi unique_ptr trong một vector khác. Tham chiếu này sẽ
    // trở nên không hợp lệ (dangling reference) sau khi hàm Refresh() được gọi
    // và con trỏ được di chuyển. Chỉ nên sử dụng tham chiếu này ngay lập tức
    // để thiết lập các component, không nên lưu trữ nó.

    return *e;
}
