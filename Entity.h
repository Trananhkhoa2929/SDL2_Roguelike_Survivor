#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <map>
#include <typeindex> // <--- You need this include!
#include "IComponent.h"

class Entity
{
public:
	Entity() {}
	~Entity() {}

	void Update(float deltaTime)
	{
		for (auto& c : m_Components) {
			c->Update(deltaTime);
		}
	}

	void Render()
	{
		// REFACTOR: Việc render thường được xử lý bởi các RenderSystem chuyên dụng,
		// thay vì gọi trực tiếp từ Entity. Tuy nhiên, giữ lại cấu trúc này
		// vẫn ổn cho các dự án nhỏ hơn.

		for (auto& c : m_Components) {
			c->Render();
		}
	}

	bool IsActive() const { return m_IsActive; }
	void Destroy() { m_IsActive = false; }

	// Template to check if a component exists
	template<typename T>
	bool HasComponent() const {
		// Use std::type_index for comparison
		return m_ComponentMap.count(std::type_index(typeid(T)));
	}

	// Template to add a new component to the Entity
	template<typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args) {
		// REFACTOR: Sử dụng std::make_unique sẽ an toàn hơn về mặt ngoại lệ
		// so với việc dùng new và sau đó mới bọc trong unique_ptr.
		// Tuy nhiên, cách làm hiện tại vẫn hoạt động.

		T* newComponent(new T(std::forward<TArgs>(args)...));
		newComponent->entity = this;
		std::unique_ptr<IComponent> uPtr{ newComponent };
		m_Components.emplace_back(std::move(uPtr));

		// Use std::type_index as the map key
		m_ComponentMap[std::type_index(typeid(T))] = newComponent;

		newComponent->Init();
		return *newComponent;
	}

	template<typename T>
	T& GetComponent() const {
		// NOTE: Phương thức này sử dụng.at(), sẽ ném ra một ngoại lệ (throw exception)
		// nếu component không được tìm thấy. Điều này có thể làm crash game.
		// Nên sử dụng nó khi bạn chắc chắn 100% component tồn tại.

		// Use std::type_index for lookup
		auto ptr = m_ComponentMap.at(std::type_index(typeid(T)));
		return *static_cast<T*>(ptr);
	}

	// NEW: Thêm một phương thức an toàn hơn để lấy component.
	// Trả về nullptr nếu không tìm thấy, cho phép kiểm tra một cách an toàn.
	template<typename T>
	T* TryGetComponent() const {
		auto it = m_ComponentMap.find(std::type_index(typeid(T)));
		if (it != m_ComponentMap.end()) {
			return static_cast<T*>(it->second);
		}
		return nullptr;
	}

private:
	bool m_IsActive = true;
	std::vector<std::unique_ptr<IComponent>> m_Components;
	// Change the map key type to std::type_index
	std::map<std::type_index, IComponent*> m_ComponentMap;
};