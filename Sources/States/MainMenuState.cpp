#include "MainMenuState.h"
#include "Engine.h"
#include "PlayState.h"
#include "AssetManager.h"
#include "InputHandler.h"
#include "UpgradeShopState.h"
#include <iostream>
#include <sstream> // THÊM DÒNG NÀY


// Đặt hàm này ở đầu file MainMenuState.cpp
void RenderTextWrapped(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color, int maxWidth, int startX, int startY) {
    if (!font) return;

    std::stringstream ss(text);
    std::string word;
    std::string currentLine;
    int currentY = startY;

    while (ss >> word) {
        // Thử thêm từ mới vào dòng hiện tại
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        int textW, textH;
        TTF_SizeText(font, testLine.c_str(), &textW, &textH);

        // Nếu dòng vượt quá chiều rộng cho phép
        if (textW > maxWidth) {
            // Vẽ dòng hiện tại (nếu có)
            if (!currentLine.empty()) {
                SDL_Surface* surface = TTF_RenderUTF8_Blended(font, currentLine.c_str(), color);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
                SDL_Rect destRect = { startX, currentY, textW, textH };
                SDL_RenderCopy(renderer, texture, nullptr, &destRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }
            // Bắt đầu dòng mới với từ hiện tại
            currentLine = word;
            currentY += textH; // Di chuyển xuống dòng tiếp theo
        }
        else {
            // Nếu không, tiếp tục thêm từ vào dòng
            currentLine = testLine;
        }
    }

    // Vẽ dòng cuối cùng còn lại
    if (!currentLine.empty()) {
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, currentLine.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        int textW, textH;
        SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
        SDL_Rect destRect = { startX, currentY, textW, textH };
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void MainMenuState::OnEnter() {
    std::cout << "Entering MainMenuState..." << std::endl;

    // SỬA: Thêm logic load font ngay trong MainMenuState.
    // Trạng thái này cần font để vẽ, nên nó phải tự load.
    // QUAN TRỌNG: Hãy đảm bảo bạn đã đổi sang đường dẫn tương đối!
    if (!AssetManager::GetInstance()->LoadFont("main_font", "Asset/font/your_font.ttf", 32)) {
        std::cerr << "!!! CRITICAL: FAILED TO LOAD MAIN FONT IN MAIN MENU !!!" << std::endl;
    }

    // Lấy 3 vũ khí ngẫu nhiên từ một danh sách định sẵn để làm lựa chọn khởi đầu
    // Tạm thời chúng ta sẽ định nghĩa chúng ở đây
    m_startingWeaponChoices.push_back({ "new_weapon_magic_wand", "Magic Wand", "Automatically shoots at the nearest enemy.", UpgradeData::Category::NEW_WEAPON, "magic_wand", 0, false });
    m_startingWeaponChoices.push_back({ "new_weapon_whip", "Whip", "Whips horizontally in front.", UpgradeData::Category::NEW_WEAPON, "whip", 0, false });
    m_startingWeaponChoices.push_back({ "new_weapon_garlic", "Garlic", "Aura that deals continuous damage.", UpgradeData::Category::NEW_WEAPON, "garlic", 0, false });

    // (Nâng cao) Trong tương lai, bạn có thể có một danh sách lớn hơn và lấy ngẫu nhiên 3 cái từ đó.
    // Định nghĩa nút Nâng Cấp
    m_upgradeButtonRect = { (1280 - 250) / 2, 550, 250, 60 };
}

void MainMenuState::OnExit() {
    std::cout << "Exiting MainMenuState..." << std::endl;
}

void MainMenuState::Update(float deltaTime) {
    // Trong menu, chúng ta chỉ cần xử lý input
    HandleSelection();
}

void MainMenuState::Render() {
    SDL_Renderer* renderer = Engine::GetInstance()->GetRenderer();

    // Vẽ nền cho menu
    SDL_SetRenderDrawColor(renderer, 15, 23, 42, 255); // Màu xanh đen
    SDL_RenderClear(renderer);

    // Vẽ tiêu đề
    AssetManager* assets = AssetManager::GetInstance();
    SDL_Color titleColor = { 255, 255, 255, 255 };
    SDL_Texture* titleTex = assets->GetTextTexture("CHOOSE STARTING WEAPON", "main_font", titleColor);
    if (titleTex) {
        int texW, texH;
        SDL_QueryTexture(titleTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect destRect = { (1280 - texW) / 2, 150, texW, texH };
        SDL_RenderCopy(renderer, titleTex, nullptr, &destRect);
    }

    // Vẽ các thẻ lựa chọn vũ khí (tương tự RenderUpgradeUI)
    const int cardWidth = 300;
    const int cardHeight = 150;
    const int cardSpacing = 30;
    const int numCards = m_startingWeaponChoices.size();
    const int totalWidth = (cardWidth * numCards) + (cardSpacing * (numCards - 1));
    int startX = (1280 - totalWidth) / 2;
    int startY = (720 - cardHeight) / 2;

    SDL_Color cardTitleColor = { 255, 215, 0, 255 };
    SDL_Color cardDescColor = { 220, 220, 220, 255 };

    m_weaponCardRects.clear();

    for (size_t i = 0; i < numCards; ++i) {
        SDL_Rect cardRect = { startX + static_cast<int>(i) * (cardWidth + cardSpacing), startY, cardWidth, cardHeight };
        m_weaponCardRects.push_back(cardRect);
        // ... logic vẽ thẻ và chữ tương tự như PlayState::RenderUpgradeUI ...
        // Vẽ nền thẻ
        SDL_SetRenderDrawColor(renderer, 45, 52, 71, 255); // Màu xanh than đậm
        SDL_RenderFillRect(renderer, &cardRect);
        // Vẽ viền thẻ
        SDL_SetRenderDrawColor(renderer, 85, 99, 134, 255); // Màu xanh xám
        SDL_RenderDrawRect(renderer, &cardRect);

        // Vẽ tên vũ khí
        SDL_Texture* nameTex = assets->GetTextTexture(m_startingWeaponChoices[i].name, "main_font", cardTitleColor);
        if (nameTex) {
            int texW, texH;
            SDL_QueryTexture(nameTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect destRect = { cardRect.x + 15, cardRect.y + 15, texW, texH };
            SDL_RenderCopy(renderer, nameTex, nullptr, &destRect);
        }

        // Vẽ mô tả có xuống dòng
        TTF_Font* font = assets->GetFont("main_font");
        if (font) {
            int padding = 15;
            RenderTextWrapped(renderer,
                font,
                m_startingWeaponChoices[i].description,
                cardDescColor,
                cardWidth - (padding * 2), // Chiều rộng tối đa
                cardRect.x + padding,      // Vị trí X bắt đầu
                cardRect.y + 55);          // Vị trí Y bắt đầu
        }
    }
    SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255); // Màu xanh tím
    SDL_RenderFillRect(renderer, &m_upgradeButtonRect);
    SDL_Texture* upgradeTex = assets->GetTextTexture("Upgrades", "main_font", titleColor);
    if (upgradeTex) {
        int texW, texH;
        SDL_QueryTexture(upgradeTex, nullptr, nullptr, &texW, &texH);

        // Tính toán vị trí để chữ nằm chính giữa nút
        SDL_Rect destRect = {
            m_upgradeButtonRect.x + (m_upgradeButtonRect.w - texW) / 2,
            m_upgradeButtonRect.y + (m_upgradeButtonRect.h - texH) / 2,
            texW,
            texH
        };

        SDL_RenderCopy(renderer, upgradeTex, nullptr, &destRect);
    }
}

void MainMenuState::HandleSelection() {
    if (InputHandler::GetInstance()->IsMouseButtonPressed(1)) {
        Vector2D mousePos = InputHandler::GetInstance()->GetMousePosition();
        SDL_Point mousePoint = { static_cast<int>(mousePos.x), static_cast<int>(mousePos.y) };

        int cardWidth = 300;
        int cardHeight = 150;
        int startX = (1280 - (cardWidth * m_startingWeaponChoices.size() + 20 * (m_startingWeaponChoices.size() - 1))) / 2;
        int startY = (720 - cardHeight) / 2;

        for (size_t i = 0; i < m_startingWeaponChoices.size(); ++i) {
            SDL_Rect cardRect = { startX + static_cast<int>(i) * (cardWidth + 20), startY, cardWidth, cardHeight };
            if (mousePos.x >= cardRect.x && mousePos.x <= cardRect.x + cardRect.w &&
                mousePos.y >= cardRect.y && mousePos.y <= cardRect.y + cardRect.h) {
                std::string chosenWeaponId = m_startingWeaponChoices[i].targetStat;
                // Chuyển sang PlayState và truyền vũ khí đã chọn
                Engine::GetInstance()->GetStateMachine()->ChangeState(new PlayState(chosenWeaponId));
                break;
            }
        }
        // Thêm logic kiểm tra click vào nút Nâng Cấp
        if (SDL_PointInRect(&mousePoint, &m_upgradeButtonRect)) {
            Engine::GetInstance()->GetStateMachine()->ChangeState(new UpgradeShopState());
        }
    }
}