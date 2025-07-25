// src/ECS/Components/ColliderComponent.h

#pragma once
#include <string>
#include <SDL.h>
#include "IComponent.h"

class Entity; // Vẫn giữ khai báo trước

class ColliderComponent : public IComponent
{
public:
	SDL_Rect hitbox;
	std::string tag;

	// Chỉ khai báo constructor
	ColliderComponent(const std::string& tag);

	// Chỉ khai báo các hàm, không có phần thân hàm
	void Init() override;
	void Update(float deltaTime) override;
};