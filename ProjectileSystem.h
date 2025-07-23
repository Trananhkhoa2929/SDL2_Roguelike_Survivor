#pragma once

// src/Systems/ProjectileSystem.h

#pragma once

class EntityManager;

class ProjectileSystem
{
public:
    void Update(EntityManager& entityManager, float deltaTime);
};