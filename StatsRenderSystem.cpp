// File: src/Systems/StatsRenderSystem.cpp (M?I)

#include "StatsRenderSystem.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "PlayerControllerComponent.h"
#include "StatsComponent.h"
#include "HealthComponent.h"
#include <string>
#include <vector>
#include <iomanip> // C?n thi?t ?? ??nh d?ng s? th?p ph�n
#include <sstream> // C?n thi?t ?? chuy?n s? th�nh chu?i v?i ??nh d?ng

void StatsRenderSystem::Render(EntityManager& entityManager, SDL_Renderer* renderer) {
    // 1. T�m ng??i ch?i v� c�c component c?n thi?t c?a h?
    Entity* player = nullptr;
    StatsComponent* stats = nullptr;
    HealthComponent* health = nullptr;

    for (auto& entity : entityManager.GetEntities()) {
        if (entity->IsActive() && entity->HasComponent<PlayerControllerComponent>()) {
            player = entity.get();
            if (player->HasComponent<StatsComponent>()) {
                stats = &player->GetComponent<StatsComponent>();
            }
            if (player->HasComponent<HealthComponent>()) {
                health = &player->GetComponent<HealthComponent>();
            }
            break; // Gi? s? ch? c� m?t ng??i ch?i
        }
    }

    // N?u kh�ng t�m th?y ng??i ch?i ho?c c�c component c?n thi?t, kh�ng l�m g� c?
    if (!player || !stats || !health) {
        return;
    }

    // 2. Chu?n b? d? li?u v� v? tr� v?
    AssetManager* assets = AssetManager::GetInstance();
    SDL_Color textColor = { 255, 255, 255, 255 }; // M�u tr?ng
    int x = 1280 - 300; // V? tr� b?t ??u v? ? g�c tr�n b�n ph?i
    int y = 20;
    int yOffset = 25; // Kho?ng c�ch gi?a c�c d�ng

    // S? d?ng stringstream ?? ??nh d?ng s? th?p ph�n m?t c�ch g?n g�ng
    std::stringstream ss;

    // 3. T?o m?t danh s�ch c�c d�ng text c?n v?
    std::vector<std::string> statsLines;

    // M�u
    statsLines.push_back("HP: " + std::to_string(health->currentHealth) + " / " + std::to_string(health->maxHealth));

    // T?c ?? di chuy?n
    ss.str(""); // X�a n?i dung c? c?a stringstream
    ss << std::fixed << std::setprecision(0) << stats->GetCurrentMoveSpeed();
    statsLines.push_back("Move Speed: " + ss.str());

    // S�t th??ng (hi?n th? d??i d?ng %)
    ss.str("");
    ss << std::fixed << std::setprecision(0) << stats->damageMultiplier * 100 << "%";
    statsLines.push_back("Damage: " + ss.str());

    // Gi?m H?i chi�u (hi?n th? d??i d?ng %)
    ss.str("");
    ss << std::fixed << std::setprecision(0) << (1.0f - stats->cooldownReduction) * 100 << "%";
    statsLines.push_back("Cooldown Reduction: " + ss.str());

    // V�ng ?nh h??ng (hi?n th? d??i d?ng %)
    ss.str("");
    ss << std::fixed << std::setprecision(0) << stats->areaSizeMultiplier * 100 << "%";
    statsLines.push_back("Area: " + ss.str());

    // S? l??ng ??n th�m
    statsLines.push_back("Amount Bonus: +" + std::to_string(stats->projectileCountBonus));


    // 4. V? t?ng d�ng text l�n m�n h�nh
    for (const auto& line : statsLines) {
        SDL_Texture* textTex = assets->GetTextTexture(line, "main_font", textColor);
        if (textTex) {
            int texW, texH;
            SDL_QueryTexture(textTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect destRect = { x, y, texW, texH };
            SDL_RenderCopy(renderer, textTex, nullptr, &destRect);
            y += yOffset; // Di chuy?n xu?ng d�ng ti?p theo ?? v? d�ng k? ti?p
        }
    }
}