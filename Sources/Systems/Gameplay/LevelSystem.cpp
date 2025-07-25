#include "LevelSystem.h"
#include "EntityManager.h"
#include "PlayerControllerComponent.h"
#include "LevelComponent.h"
#include "PlayState.h"

void LevelSystem::Update(EntityManager& entityManager, PlayState* playState, float deltaTime)
{
    if (!playState) return;

    for (auto& entity : entityManager.GetEntities())
    {
        if (entity->HasComponent<PlayerControllerComponent>() && entity->HasComponent<LevelComponent>())
        {
            auto& level = entity->GetComponent<LevelComponent>();
            if (level.CanLevelUp())
            {
                level.LevelUp();

                // TODO: Tạm dừng game và hiển thị màn hình chọn nâng cấp.
                // Hiện tại, chúng ta chỉ in ra console.
                // Ví dụ: Engine::GetInstance()->PauseGame();
                playState->TriggerLevelUpScreen();
            }
            break; // Chỉ có một người chơi
        }
    }
}