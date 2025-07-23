// File: src/ECS/Components/GoldCoinComponent.h (MỚI)
#pragma once
#include "IComponent.h"

class GoldCoinComponent : public IComponent {
public:
    int value;
    GoldCoinComponent(int v = 1) : value(v) {}
};