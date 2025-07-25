// File: src/Systems/TimeRenderSystem.cpp (MỚI)
#include "TimeRenderSystem.h"
#include "AssetManager.h"
#include <string>
#include <iomanip> // Cần cho std::setw và std::setfill
#include <sstream> // Cần cho std::stringstream

void TimeRenderSystem::Render(float gameTime, SDL_Renderer* renderer) {
    // 1. Tính toán phút và giây từ tổng số giây
    int totalSeconds = static_cast<int>(gameTime);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // 2. Định dạng chuỗi thành dạng "MM:SS"
    std::stringstream timeStream;
    timeStream << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;
    std::string timeText = timeStream.str();

    // 3. Chuẩn bị để vẽ
    AssetManager* assets = AssetManager::GetInstance();
    SDL_Color textColor = { 255, 255, 255, 255 }; // Màu trắng

    // 4. Lấy texture từ AssetManager và vẽ nó
    SDL_Texture* textTex = assets->GetTextTexture(timeText, "main_font", textColor);
    if (textTex) {
        int texW, texH;
        SDL_QueryTexture(textTex, nullptr, nullptr, &texW, &texH);

        // Vị trí ở chính giữa, phía trên màn hình
        SDL_Rect destRect = { (1280 - texW) / 2, 20, texW, texH };
        SDL_RenderCopy(renderer, textTex, nullptr, &destRect);
    }
}