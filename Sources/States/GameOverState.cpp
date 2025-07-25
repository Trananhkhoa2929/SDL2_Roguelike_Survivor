// File: src/States/GameOverState.cpp (MỚI)
#include "GameOverState.h"
#include "Engine.h"
#include "MainMenuState.h" // Cần để quay về menu chính
#include "AssetManager.h"
#include "InputHandler.h"
#include <iostream>
#include <string>

GameOverState::GameOverState(int goldEarned) : m_goldEarned(goldEarned) {}

void GameOverState::OnEnter() {
    std::cout << "Entering GameOverState..." << std::endl;

    // Định nghĩa vị trí và kích thước cho các nút
    const int buttonWidth = 250;
    const int buttonHeight = 60;
    const int screenCenterX = 1280 / 2;
    const int screenCenterY = 720 / 2;

    m_replayButtonRect = { screenCenterX - buttonWidth / 2, screenCenterY, buttonWidth, buttonHeight };
    m_exitButtonRect = { screenCenterX - buttonWidth / 2, screenCenterY + buttonHeight + 20, buttonWidth, buttonHeight };
}

void GameOverState::OnExit() {
    std::cout << "Exiting GameOverState..." << std::endl;
}

void GameOverState::Update(float deltaTime) {
    HandleInput();
}

void GameOverState::Render() {
    SDL_Renderer* renderer = Engine::GetInstance()->GetRenderer();
    AssetManager* assets = AssetManager::GetInstance();

    // 1. Vẽ một lớp nền mờ để làm nổi bật UI
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // Mờ hơn một chút
    SDL_RenderFillRect(renderer, nullptr);

    // 2. Vẽ chữ "GAME OVER"
    SDL_Color titleColor = { 220, 20, 60, 255 }; // Màu đỏ thẫm
    SDL_Texture* titleTex = assets->GetTextTexture("GAME OVER", "main_font", titleColor);
    if (titleTex) {
        int texW, texH;
        SDL_QueryTexture(titleTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = { (1280 - texW) / 2, 200, texW, texH };
        SDL_RenderCopy(renderer, titleTex, nullptr, &destRect);
    }

    // Vẽ số vàng kiếm được
    std::string goldText = "Gold Earned: " + std::to_string(m_goldEarned);
    SDL_Texture* goldTex = assets->GetTextTexture(goldText, "main_font", { 255, 215, 0, 255 });
    if (goldTex) {
        int texW, texH;
        SDL_QueryTexture(goldTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = { (1280 - texW) / 2, 300, texW, texH };
        SDL_RenderCopy(renderer, goldTex, nullptr, &destRect);
    }

    // 3. Vẽ nút "Chơi Lại"
    SDL_SetRenderDrawColor(renderer, 45, 200, 85, 255); // Màu xanh lá
    SDL_RenderFillRect(renderer, &m_replayButtonRect);
    SDL_Color buttonTextColor = { 255, 255, 255, 255 };
    SDL_Texture* replayTex = assets->GetTextTexture("Play Again", "main_font", buttonTextColor);
    if (replayTex) {
        int texW, texH;
        SDL_QueryTexture(replayTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = { m_replayButtonRect.x + (m_replayButtonRect.w - texW) / 2, m_replayButtonRect.y + (m_replayButtonRect.h - texH) / 2, texW, texH };
        SDL_RenderCopy(renderer, replayTex, nullptr, &destRect);
    }

    // 4. Vẽ nút "Thoát"
    SDL_SetRenderDrawColor(renderer, 200, 45, 45, 255); // Màu đỏ
    SDL_RenderFillRect(renderer, &m_exitButtonRect);
    SDL_Texture* exitTex = assets->GetTextTexture("Exit", "main_font", buttonTextColor);
    if (exitTex) {
        int texW, texH;
        SDL_QueryTexture(exitTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = { m_exitButtonRect.x + (m_exitButtonRect.w - texW) / 2, m_exitButtonRect.y + (m_exitButtonRect.h - texH) / 2, texW, texH };
        SDL_RenderCopy(renderer, exitTex, nullptr, &destRect);
    }
}

void GameOverState::HandleInput() {
    if (InputHandler::GetInstance()->IsMouseButtonPressed(1)) {
        Vector2D mousePos = InputHandler::GetInstance()->GetMousePosition();
        SDL_Point mousePoint = { static_cast<int>(mousePos.x), static_cast<int>(mousePos.y) };

        if (SDL_PointInRect(&mousePoint, &m_replayButtonRect)) {
            std::cout << "Replay button clicked!" << std::endl;
            Engine::GetInstance()->GetStateMachine()->ChangeState(new MainMenuState());
        }
        else if (SDL_PointInRect(&mousePoint, &m_exitButtonRect)) {
            std::cout << "Exit button clicked!" << std::endl;
            Engine::GetInstance()->Quit();
        }
    }
}

