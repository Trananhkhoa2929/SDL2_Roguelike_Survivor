#pragma once
#include "IComponent.h"

// MỚI: Component này đánh dấu một thực thể là một viên kinh nghiệm có thể nhặt được.

class ExperienceOrbComponent : public IComponent
{
public:
	int xpValue;

	ExperienceOrbComponent(int value) : xpValue(value) {}
};