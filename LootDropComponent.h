#pragma once
#include "IComponent.h"

// MỚI: Component này gắn vào kẻ thù, xác định chúng sẽ rơi ra gì khi chết.
// Hiện tại chỉ rơi ra điểm kinh nghiệm (XP).
class LootDropComponent : public IComponent
{
public:
	int xpAmount; 
	int goldAmount;

	LootDropComponent(int xp = 10, int gold = 1) : xpAmount(xp), goldAmount(gold) {}
};
