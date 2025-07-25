#pragma once

#include "IComponent.h"

// MỚI: Component này chứa số điểm mà một thực thể sẽ mang lại khi bị tiêu diệt.
class ScoreValueComponent : public IComponent {
public:
    int scoreValue;

    ScoreValueComponent(int value = 0) : scoreValue(value) {}
};