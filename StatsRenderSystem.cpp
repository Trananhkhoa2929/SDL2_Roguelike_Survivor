// File: src/Systems/StatsRenderSystem.cpp (M?I)

#include "StatsRenderSystem.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "PlayerControllerComponent.h"
#include "StatsComponent.h"
#include "HealthComponent.h"
#include <string>
#include <vector>
#include <iomanip> // C?n thi?t ?? ??nh d?ng s? th?p phân
#include <sstream> // C?n thi?t ?? chuy?n s? thành chu?i v?i ??nh d?ng

void StatsRenderSystem::Render(EntityManager& entityManager, SDL_Renderer* renderer) {
    // 1. Tìm ng??i ch?i và các component c?n thi?t c?a h?
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
            break; // Gi? s? ch? có m?t ng??i ch?i
        }
    }

    // N?u không tìm th?y ng??i ch?i ho?c các component c?n thi?t, không làm gì c?
    if (!player || !stats || !health) {
        return;
    }

    // 2. Chu?n b? d? li?u và v? trí v?
    AssetManager* assets = AssetManager::GetInstance();
    SDL_Color textColor = { 255, 255, 255, 255 }; // Màu tr?ng
    int x = 1280 - 300; // V? trí b?t ??u v? ? góc trên bên ph?i
    int y = 20;
    int yOffset = 25; // Kho?ng cách gi?a các dòng

    // S? d?ng stringstream ?? ??nh d?ng s? th?p phân m?t cách g?n gàng
    std::stringstream ss;

    // 3. T?o m?t danh sách các dòng text c?n v?
    std::vector<std::string> statsLines;

    // Máu
    statsLines.push_back("HP: " + std::to_string(health->currentHealth) + " / " + std::to_string(health->maxHealth));

    // T?c ?? di chuy?n
    ss.str(""); // Xóa n?i dung c? c?a stringstream
    ss << std::fixed << std::setprecision(0) << stats->GetCurrentMoveSpeed();
    statsLines.push_back("Move Speed: " + ss.str());

    // Sát th??ng (hi?n th? d??i d?ng %)
    ss.str("");
    ss << std::fixed << std::setprecision(0) << stats->damageMultiplier * 100 << "%";
    statsLines.push_back("Damage: " + ss.str());

    // Gi?m H?i chiêu (hi?n th? d??i d?ng %)
    ss.str("");
    ss << std::fixed << std::setprecision(0) << (1.0f - stats->cooldownReduction) * 100 << "%";
    statsLines.push_back("Cooldown Reduction: " + ss.str());

    // Vùng ?nh h??ng (hi?n th? d??i d?ng %)
    ss.str("");
    ss << std::fixed << std::setprecision(0) << stats->areaSizeMultiplier * 100 << "%";
    statsLines.push_back("Area: " + ss.str());

    // S? l??ng ??n thêm
    statsLines.push_back("Amount Bonus: +" + std::to_string(stats->projectileCountBonus));


    // 4. V? t?ng dòng text lên màn hình
    for (const auto& line : statsLines) {
        SDL_Texture* textTex = assets->GetTextTexture(line, "main_font", textColor);
        if (textTex) {
            int texW, texH;
            SDL_QueryTexture(textTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect destRect = { x, y, texW, texH };
            SDL_RenderCopy(renderer, textTex, nullptr, &destRect);
            y += yOffset; // Di chuy?n xu?ng dòng ti?p theo ?? v? dòng k? ti?p
        }
    }
}