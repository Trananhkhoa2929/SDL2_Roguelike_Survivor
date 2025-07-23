// File: src/States/PauseState.cpp (MỚI)
#include "PauseState.h"
#include "Engine.h"
#include "MainMenuState.h"
#include "AssetManager.h"
#include "InputHandler.h"
#include <iostream>

void PauseState::OnEnter() {
    std::cout << "Entering PauseState..." << std::endl;
    const int buttonWidth = 250;
    const int buttonHeight = 60;
    const int screenCenterX = 1280 / 2;
    const int screenCenterY = 720 / 2;
    m_continueButtonRect = { screenCenterX - buttonWidth / 2, screenCenterY - buttonHeight, buttonWidth, buttonHeight };
    m_mainMenuButtonRect = { screenCenterX - buttonWidth / 2, screenCenterY + 20, buttonWidth, buttonHeight };
}

void PauseState::OnExit() {
    std::cout << "Exiting PauseState..." << std::endl;
}

void PauseState::Update(float deltaTime) {
    HandleInput();
}

void PauseState::Render() {
    SDL_Renderer* renderer = Engine::GetInstance()->GetRenderer();
    AssetManager* assets = AssetManager::GetInstance();

    // Vẽ lớp nền mờ (quan trọng)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, nullptr);

    // Vẽ chữ "PAUSING"
    SDL_Color titleColor = { 255, 255, 255, 255 };
    SDL_Texture* titleTex = assets->GetTextTexture("PAUSED", "main_font", titleColor);
    if (titleTex) {
        int texW, texH;
        SDL_QueryTexture(titleTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = { (1280 - texW) / 2, 200, texW, texH };
        SDL_RenderCopy(renderer, titleTex, nullptr, &destRect);
    }

    // Vẽ nút "Choi Tiep"
    SDL_SetRenderDrawColor(renderer, 45, 200, 85, 255);
    SDL_RenderFillRect(renderer, &m_continueButtonRect);
    SDL_Texture* continueTex = assets->GetTextTexture("Continue", "main_font", titleColor);
    if (continueTex) {
        int texW, texH;
        SDL_QueryTexture(continueTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = {
            m_continueButtonRect.x + (m_continueButtonRect.w - texW) / 2,
            m_continueButtonRect.y + (m_continueButtonRect.h - texH) / 2,
            texW,
            texH
        };
        SDL_RenderCopy(renderer, continueTex, nullptr, &destRect);
    }
    // Vẽ nút "Menu Chinh"
    SDL_SetRenderDrawColor(renderer, 200, 45, 45, 255);
    SDL_RenderFillRect(renderer, &m_mainMenuButtonRect);
    SDL_Texture* menuTex = assets->GetTextTexture("Main Menu", "main_font", titleColor);
    if (menuTex) { 
        int texW, texH;
        SDL_QueryTexture(menuTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = {
            m_mainMenuButtonRect.x + (m_mainMenuButtonRect.w - texW) / 2,
            m_mainMenuButtonRect.y + (m_mainMenuButtonRect.h - texH) / 2,
            texW,
            texH
        };
        SDL_RenderCopy(renderer, menuTex, nullptr, &destRect);
    }
}

void PauseState::HandleInput() {
    // Nếu người chơi bấm ESC lần nữa, cũng sẽ thoát khỏi pause
    if (InputHandler::GetInstance()->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        Engine::GetInstance()->GetStateMachine()->PopState();
        return;
    }

    if (InputHandler::GetInstance()->IsMouseButtonPressed(1)) {
        Vector2D mousePos = InputHandler::GetInstance()->GetMousePosition();
        SDL_Point mousePoint = { static_cast<int>(mousePos.x), static_cast<int>(mousePos.y) };

        // Kiểm tra click vào nút "Choi Tiep"
        if (SDL_PointInRect(&mousePoint, &m_continueButtonRect)) {
            Engine::GetInstance()->GetStateMachine()->PopState();
        }
        // Kiểm tra click vào nút "Menu Chinh"
        if (SDL_PointInRect(&mousePoint, &m_mainMenuButtonRect)) {
            // Dọn dẹp toàn bộ stack và quay về menu
            Engine::GetInstance()->GetStateMachine()->Clean();
            Engine::GetInstance()->GetStateMachine()->PushState(new MainMenuState());
        }
    }
}