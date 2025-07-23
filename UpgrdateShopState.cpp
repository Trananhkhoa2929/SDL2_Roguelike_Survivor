#include "UpgradeShopState.h"
#include "Engine.h"
#include "MainMenuState.h"
#include "SaveSystem.h"
#include "AssetManager.h"
#include "InputHandler.h"
#include <iostream>
#include <string>

void UpgradeShopState::OnEnter() {
    std::cout << "Entering UpgradeShopState..." << std::endl;
    PopulateUpgrades();
    m_backButtonRect = { 20, 650, 200, 50 };
}

void UpgradeShopState::OnExit() {
    std::cout << "Exiting UpgradeShopState..." << std::endl;
}

void UpgradeShopState::Update(float deltaTime) {
    HandleInput();
}

void UpgradeShopState::Render() {
    SDL_Renderer* renderer = Engine::GetInstance()->GetRenderer();
    AssetManager* assets = AssetManager::GetInstance();
    SaveSystem* save = SaveSystem::GetInstance();

    // Vẽ nền
    SDL_SetRenderDrawColor(renderer, 15, 23, 42, 255);
    SDL_RenderClear(renderer);

    // Vẽ UI Vàng
    RenderGoldUI(renderer);

    // Vẽ danh sách nâng cấp
    int startX = 50;
    int startY = 100;
    SDL_Color nameColor = { 255, 215, 0, 255 };
    SDL_Color descColor = { 200, 200, 200, 255 };
    SDL_Color levelColor = { 150, 150, 255, 255 };
    SDL_Color costColor = { 255, 255, 100, 255 };
    SDL_Color buttonTextColor = { 255, 255, 255, 255 };
    SDL_Color maxLevelColor = { 100, 255, 100, 255 };

    for (auto& upgrade : m_upgrades) {
        int currentLevel = save->GetUpgradeLevel(upgrade.id);

        // Vẽ tên và mô tả
        SDL_Texture* nameTex = assets->GetTextTexture(upgrade.name, "main_font", nameColor);
        if (nameTex) { 
            int texW, texH;
            SDL_QueryTexture(nameTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect destRect = { startX, startY, texW, texH };
            SDL_RenderCopy(renderer, nameTex, nullptr, &destRect);
        }

        SDL_Texture* descTex = assets->GetTextTexture(upgrade.description, "main_font", descColor);
        if (descTex) { 
            int texW, texH;
            SDL_QueryTexture(descTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect destRect = { startX, startY + 30, texW, texH };
            SDL_RenderCopy(renderer, descTex, nullptr, &destRect);
        }

        // Vẽ cấp độ và giá tiền
        std::string levelText = "Level: " + std::to_string(currentLevel) + " / " + std::to_string(upgrade.maxLevel);
        SDL_Texture* levelTex = assets->GetTextTexture(levelText, "main_font", levelColor);
        if (levelTex) {
            int texW, texH;
            SDL_QueryTexture(levelTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect destRect = { startX + 400, startY + 10, texW, texH };
            SDL_RenderCopy(renderer, levelTex, nullptr, &destRect);
        }

        if (currentLevel < upgrade.maxLevel) 
        {
            int cost = upgrade.costFunction(currentLevel);
            std::string costText = "Cost: " + std::to_string(cost) + " Gold";
            SDL_Texture* costTex = assets->GetTextTexture(costText, "main_font", costColor);
            if (costTex) {
                int texW, texH;
                SDL_QueryTexture(costTex, nullptr, nullptr, &texW, &texH);
                SDL_Rect destRect = { startX + 600, startY + 10, texW, texH };
                SDL_RenderCopy(renderer, costTex, nullptr, &destRect);
            }

            // Vẽ nút Mua
            upgrade.buyButtonRect = { 1000, startY, 150, 40 };
            SDL_SetRenderDrawColor(renderer, 45, 200, 85, 255);
            SDL_RenderFillRect(renderer, &upgrade.buyButtonRect);
            SDL_Texture* buyTex = assets->GetTextTexture("Buy", "main_font", buttonTextColor);
            if (buyTex) {
                int texW, texH;
                SDL_QueryTexture(buyTex, nullptr, nullptr, &texW, &texH);
                SDL_Rect destRect = { upgrade.buyButtonRect.x + (upgrade.buyButtonRect.w - texW) / 2, upgrade.buyButtonRect.y + (upgrade.buyButtonRect.h - texH) / 2, texW, texH };
                SDL_RenderCopy(renderer, buyTex, nullptr, &destRect);
            }
        }
        else {
            SDL_Texture* maxTex = assets->GetTextTexture("MAX LEVEL", "main_font", maxLevelColor);
            if (maxTex) {
                int texW, texH;
                SDL_QueryTexture(maxTex, nullptr, nullptr, &texW, &texH);
                SDL_Rect destRect = { 1000, startY + 5, texW, texH };
                SDL_RenderCopy(renderer, maxTex, nullptr, &destRect);
            }
        }
        startY += 80;
    }

    // Vẽ nút Quay Lại
    SDL_SetRenderDrawColor(renderer, 200, 45, 45, 255);
    SDL_RenderFillRect(renderer, &m_backButtonRect);
    SDL_Texture* backTex = assets->GetTextTexture("Back", "main_font", buttonTextColor);
    if (backTex) {
        int texW, texH;
        SDL_QueryTexture(backTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = { m_backButtonRect.x + (m_backButtonRect.w - texW) / 2, m_backButtonRect.y + (m_backButtonRect.h - texH) / 2, texW, texH };
        SDL_RenderCopy(renderer, backTex, nullptr, &destRect);
    }
}

void UpgradeShopState::HandleInput() {
    if (InputHandler::GetInstance()->IsMouseButtonPressed(1)) {
        Vector2D mousePos = InputHandler::GetInstance()->GetMousePosition();
        SDL_Point mousePoint = { static_cast<int>(mousePos.x), static_cast<int>(mousePos.y) };

        for (const auto& upgrade : m_upgrades) {
            if (SDL_PointInRect(&mousePoint, &upgrade.buyButtonRect)) {
                SaveSystem* save = SaveSystem::GetInstance();
                int currentLevel = save->GetUpgradeLevel(upgrade.id);
                if (currentLevel < upgrade.maxLevel) {
                    int cost = upgrade.costFunction(currentLevel);
                    if (save->SpendGold(cost)) {
                        save->IncreaseUpgradeLevel(upgrade.id);
                        save->SaveGame();
                        std::cout << "Purchased " << upgrade.name << std::endl;
                    }
                    else {
                        std::cout << "Not enough gold!" << std::endl;
                    }
                }
            }
        }
        if (SDL_PointInRect(&mousePoint, &m_backButtonRect)) {
            Engine::GetInstance()->GetStateMachine()->ChangeState(new MainMenuState());
        }
    }
}

void UpgradeShopState::PopulateUpgrades() {
    m_upgrades.push_back({
        "perm_damage", "Permanent Strength", "+5% starting damage per level.", 5,
        [](int level) { return 100 + level * 150; }
        });
    m_upgrades.push_back({
        "perm_health", "Permanent Vigor", "+10% starting health per level.", 5,
        [](int level) { return 100 + level * 150; }
        });
}

void UpgradeShopState::RenderGoldUI(SDL_Renderer* renderer) {
    std::string goldText = "Gold: " + std::to_string(SaveSystem::GetInstance()->GetGold());
    SDL_Texture* goldTex = AssetManager::GetInstance()->GetTextTexture(goldText, "main_font", { 255, 215, 0, 255 });
    if (goldTex) {
        int texW, texH;
        SDL_QueryTexture(goldTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = { 1280 - texW - 20, 20, texW, texH };
        SDL_RenderCopy(renderer, goldTex, nullptr, &destRect);
    }
}