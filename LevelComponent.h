#pragma once
#include "IComponent.h"
#include <iostream>

class LevelComponent : public IComponent
{
public:
	int currentLevel;
	int currentXP;
	int xpToNextLevel;

	LevelComponent(int startLevel = 1) : currentLevel(startLevel), currentXP(0) {
		CalculateXPForNextLevel();
	}

	void AddXP(int amount) {
		currentXP += amount;
		std::cout << "Nhan duoc " << amount << " XP! Tong: " << currentXP << "/" << xpToNextLevel << std::endl;
	}

	bool CanLevelUp() const {
		return currentXP >= xpToNextLevel;
	}

	void LevelUp() {
		currentXP -= xpToNextLevel;
		currentLevel++;
		CalculateXPForNextLevel();
		std::cout << "********************************" << std::endl;
		std::cout << "*** LEN CAP! Ban da dat cap " << currentLevel << " ***" << std::endl;
		std::cout << "********************************" << std::endl;
	}

private:
	void CalculateXPForNextLevel() {
		// Công thức lên cấp đơn giản, có thể điều chỉnh để game khó hơn
		xpToNextLevel = 50 + (currentLevel * currentLevel * 50);
	}
};