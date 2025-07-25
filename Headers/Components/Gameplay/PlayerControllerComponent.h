#pragma once
#include "IComponent.h"

class PlayerControllerComponent : public IComponent
{
public:
	// SỬA: Thêm các thuộc tính cho việc nhặt đồ.
	float pickupRange = 24.0f;  // Tầm nhặt trực tiếp
	float magnetRange = 150.0f; // Tầm hút các viên XP về phía người chơi
	int sessionGold = 0;
	PlayerControllerComponent() = default;
};