// src/States/PlayState.cpp

#include "PlayState.h"
#include "GameOverState.h"
#include "AssetManager.h"
#include "Engine.h" 
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "PlayerControllerComponent.h"
#include "ColliderComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "ProjectileComponent.h"
#include "WeaponComponents.h"
#include "LevelComponent.h"
#include "StatsComponent.h"
#include "DamageTextComponent.h"
#include "InputHandler.h"
#include "SaveSystem.h"
#include "Camera.h"
#include <iostream>


// Các con trỏ này nên được khai báo là thành viên của lớp PlayState trong file .h
// thay vì biến toàn cục để quản lý tốt hơn. Tuy nhiên, để giữ nguyên cấu trúc
// hiện tại của bạn, chúng ta sẽ tạm để ở đây.
//Entity* scoreUI = nullptr;
//Entity* player = nullptr;


// SỬA: Định nghĩa constructor mới
PlayState::PlayState(const std::string& startingWeaponId)
    : m_startingWeaponId(startingWeaponId), m_playTime(0.0f)
{
    // Constructor có thể để trống, logic khởi tạo sẽ nằm trong OnEnter
}
// MỚI: Destructor
PlayState::~PlayState() {
    delete m_pMap;
    m_pMap = nullptr;
}

void PlayState::OnEnter()
{
    std::cout << "Entering PlayState..." << std::endl;

    // QUAN TRỌNG: Bạn vẫn đang sử dụng đường dẫn tuyệt đối ("D:/...").
    // Điều này sẽ gây lỗi khi chạy trên máy khác hoặc nếu thư mục không tồn tại.
    // Hãy đổi chúng thành đường dẫn tương đối.
    AssetManager::GetInstance()->LoadTexture("player_sprite", "Asset/player.png");
    AssetManager::GetInstance()->LoadTexture("chaser_sprite", "Asset/chaser.png");
    AssetManager::GetInstance()->LoadTexture("brute_sprite", "Asset/brute.png");
    // load enemy archer và arrow của nó
    AssetManager::GetInstance()->LoadTexture("archer_sprite", "Asset/archer.png");
    AssetManager::GetInstance()->LoadTexture("arrow_sprite", "Asset/arrow.png");
    // magic wand
    AssetManager::GetInstance()->LoadTexture("bullet_sprite", "Asset/bullet.png");
    // whip
    AssetManager::GetInstance()->LoadTexture("whip_sprite", "Asset/whip.png");
    // font
    AssetManager::GetInstance()->LoadFont("main_font", "Asset/font/your_font.ttf", 24);
    // SỬA: Load thêm texture cho viên XP
    AssetManager::GetInstance()->LoadTexture("xp_orb_sprite", "Asset/xp_orb.png");
    AssetManager::GetInstance()->LoadTexture("gold_coin_sprite", "Asset/coin.png");
    AssetManager::GetInstance()->LoadTexture("pause_icon", "Asset/pause_icon.png");

   // SỬA: Load tileset mới (giả sử bạn có một file chứa 4 tile này)
    AssetManager::GetInstance()->LoadTexture("world_tileset_16", "Asset/tileset_4_types_16x16.png");

    // SỬA: Tạo và TẠO RA bản đồ với các thông số mới
    m_pMap = new Map("world_tileset_16", 16); // Tile size là 16x16
    m_pMap->GenerateMap(300, 300); // Tạo ra một bản đồ 300x300 tiles



    // Tạo người chơi
    // SỬA: Sử dụng biến thành viên m_player
    m_player = &m_EntityManager.AddEntity();
    m_player->AddComponent<TransformComponent>(1500.f, 1500.f, 32, 32, 1.0);
    m_player->AddComponent<SpriteComponent>("player_sprite");
    m_player->AddComponent<PlayerControllerComponent>();
    m_player->AddComponent<ColliderComponent>("player");
    m_player->AddComponent<HealthComponent>(100);
    m_player->AddComponent<ScoreComponent>();
    m_player->AddComponent<LevelComponent>();
    m_player->AddComponent<StatsComponent>(); // SỬA: Thêm StatsComponent cho người chơi
    // SỬA: Thêm vũ khí khởi đầu dựa trên ID đã được truyền vào
    if (m_startingWeaponId == "magic_wand") {
        m_player->AddComponent<MagicWandComponent>();
    }
    else if (m_startingWeaponId == "whip") {
        m_player->AddComponent<WhipComponent>();
    }
    else if (m_startingWeaponId == "garlic") {
        m_player->AddComponent<GarlicAuraComponent>();
    }

    // MỚI: Áp dụng các nâng cấp vĩnh viễn khi bắt đầu màn chơi
    if (m_player && m_player->HasComponent<StatsComponent>()) {
        auto& stats = m_player->GetComponent<StatsComponent>();
        SaveSystem* save = SaveSystem::GetInstance();
        stats.damageMultiplier += save->GetUpgradeLevel("perm_damage") * 0.05f; // +5% mỗi cấp
        stats.maxHealthMultiplier += save->GetUpgradeLevel("perm_health") * 0.10f; // +10% mỗi cấp
        // Cập nhật lại HealthComponent
        auto& health = m_player->GetComponent<HealthComponent>();
        health.maxHealth = stats.GetCurrentMaxHealth();
        health.currentHealth = health.maxHealth;
    }


    // Tạo UI hiển thị điểm
    // SỬA: Sử dụng biến thành viên m_scoreUI
    m_scoreUI = &m_EntityManager.AddEntity();
    m_scoreUI->AddComponent<TransformComponent>(10, 10, 0, 0, 1.0);
    SDL_Color white = { 255, 255, 255, 255 };
    m_scoreUI->AddComponent<TextComponent>("Score: 0", "main_font", white, true);

    // MỚI: Tạo thực thể nút Pause
    m_pauseButton = &m_EntityManager.AddEntity();
    m_pauseButton->AddComponent<TransformComponent>(1280 - 60, 10, 48, 48, 1.0);
    m_pauseButton->AddComponent<SpriteComponent>("pause_icon", true);
    // (Nâng cao) Có thể tạo một UIButtonComponent để đánh dấu

    std::cout << "PlayState entered successfully." << std::endl;
}

void PlayState::OnExit() {
    std::cout << "Exiting PlayState..." << std::endl;
    m_scoreUI = nullptr;
    m_player = nullptr;
}

void PlayState::Update(float deltaTime)
{
    // SỬA: Thêm logic kiểm tra pause ở đầu hàm
    if (InputHandler::GetInstance()->IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        // Đẩy PauseState lên trên PlayState
        Engine::GetInstance()->GetStateMachine()->PushState(new PauseState());
        return; // Dừng update của PlayState trong frame này
    }

    // Kiểm tra click chuột vào nút pause
    if (InputHandler::GetInstance()->IsMouseButtonPressed(1) && m_pauseButton) {
        auto& buttonTransform = m_pauseButton->GetComponent<TransformComponent>();
        SDL_Rect buttonRect = {
            static_cast<int>(buttonTransform.position.x),
            static_cast<int>(buttonTransform.position.y),
            buttonTransform.width,
            buttonTransform.height
        };
        Vector2D mousePos = InputHandler::GetInstance()->GetMousePosition();
        SDL_Point mousePoint = { static_cast<int>(mousePos.x), static_cast<int>(mousePos.y) };
        if (mousePos.x >= buttonRect.x && SDL_PointInRect(&mousePoint, &buttonRect)) {
            Engine::GetInstance()->GetStateMachine()->PushState(new PauseState());
            return;
        }
    }

    // SỬA: Nếu game đang tạm dừng để nâng cấp, chỉ xử lý input cho UI
    if (m_isPausedForUpgrade) {
        HandleUpgradeSelection();
        return; // Dừng toàn bộ logic game còn lại
    }
    // ________________________________
    // 
    // 1. Cập nhật đồng hồ và hệ thống sự kiện ở đầu mỗi frame
    m_playTime += deltaTime;
    m_GameEventSystem.Update(m_playTime, m_EnemySpawnerSystem);

    // 2. Cập nhật các hệ thống "hành động"
    m_PlayerInputSystem.Update(m_EntityManager, deltaTime);
    m_EnemySpawnerSystem.Update(m_EntityManager, deltaTime);
    m_AISystem.Update(m_EntityManager, deltaTime);
    m_RangedAISystem.Update(m_EntityManager, deltaTime);
    m_WeaponControlSystem.Update(m_EntityManager, deltaTime);

    // 3. Di chuyển TẤT CẢ các thực thể
    m_EntityManager.Update(deltaTime);

    // 4. Cập nhật các hệ thống phụ thuộc vào vị trí mới
    if (m_player && m_pMap) {
        auto& playerTransform = m_player->GetComponent<TransformComponent>();
        Camera::GetInstance()->Update(playerTransform.position, m_pMap->GetWorldWidth(), m_pMap->GetWorldHeight());
    }
    m_FollowSystem.Update(m_EntityManager, deltaTime);

    // 5. Xử lý va chạm và sát thương
    m_CollisionSystem.Update(m_EntityManager);
    m_DamageSystem.Update(m_EntityManager, deltaTime);
    m_ProjectileSystem.Update(m_EntityManager, deltaTime);

    // 6. Xử lý các hiệu ứng và vòng đời
    for (auto& entity : m_EntityManager.GetEntities()) {
        if (entity->HasComponent<HitboxComponent>()) {
            auto& hitbox = entity->GetComponent<HitboxComponent>();
            hitbox.timer += deltaTime;
            if (hitbox.timer >= hitbox.duration) {
                entity->Destroy();
            }
        }
    }

    for (auto& entity : m_EntityManager.GetEntities()) {
        if (entity->IsActive() && entity->HasComponent<DamageTextComponent>()) {
            auto& damageText = entity->GetComponent<DamageTextComponent>();
            damageText.timer += deltaTime;

            // Hủy thực thể nếu hết thời gian
            if (damageText.timer >= damageText.lifeTime) {
                entity->Destroy();
                continue; // Bỏ qua phần còn lại để tránh truy cập vào component đã hủy
            }

            // Cập nhật vị trí để bay lên
            if (entity->HasComponent<TransformComponent>()) {
                auto& transform = entity->GetComponent<TransformComponent>();
                transform.position.y -= damageText.floatSpeed * deltaTime;
            }

            // Cập nhật độ mờ (alpha) để mờ dần
            if (entity->HasComponent<TextComponent>()) {
                auto& text = entity->GetComponent<TextComponent>();
                float lifeRatio = damageText.timer / damageText.lifeTime;
                // Alpha sẽ giảm từ 255 xuống 0
                text.color.a = static_cast<Uint8>(255 * (1.0f - lifeRatio));
            }
        }
    }

    // 7. Cập nhật hệ thống máu và loot
    m_HealthSystem.Update(m_EntityManager, deltaTime);
    m_LootSystem.Update(m_EntityManager, deltaTime);
    m_LevelSystem.Update(m_EntityManager, this, deltaTime);

    // 8. SỬA LỖI CRASH: Kiểm tra trạng thái người chơi SAU KHI TẤT CẢ các hệ thống đã chạy
    if (m_player && !m_player->IsActive()) {
        // Bây giờ, người chơi chỉ bị đánh dấu là 'inactive', nhưng chưa bị xóa.
        // Việc truy cập vào component của nó là AN TOÀN.

        int goldFromPickups = 0;
        if (auto* pc = m_player->TryGetComponent<PlayerControllerComponent>()) {
            goldFromPickups = pc->sessionGold;
        }

        int goldFromTime = static_cast<int>(m_playTime / 10);
        int totalGoldEarned = goldFromPickups + goldFromTime;

        SaveSystem::GetInstance()->AddGold(totalGoldEarned);
        SaveSystem::GetInstance()->SaveGame();

        // Vô hiệu hóa con trỏ và chuyển trạng thái
        m_player = nullptr;
        Engine::GetInstance()->GetStateMachine()->ChangeState(new GameOverState(totalGoldEarned));
        return; // Dừng hàm Update của PlayState tại đây
    }
    // 9. Cập nhật UI
    if (m_player && m_scoreUI)
    {
        auto& scoreComp = m_player->GetComponent<ScoreComponent>();
        auto& textComp = m_scoreUI->GetComponent<TextComponent>();
        std::string newScoreText = "Score: " + std::to_string(scoreComp.score);
        if (textComp.text != newScoreText) {
            textComp.text = newScoreText;
        }
    }

    // 10. Dọn dẹp
    m_EntityManager.Refresh();


    //// SỬA LỖI CRASH:
    //// Sau khi Refresh(), con trỏ m_player có thể đang trỏ đến vùng nhớ đã bị giải phóng.
    //// Chúng ta phải kiểm tra xem thực thể người chơi có còn tồn tại trong EntityManager hay không.
    //if (m_player) { // Chỉ kiểm tra nếu chúng ta nghĩ rằng có một người chơi
    //    bool playerStillExists = false;
    //    for (const auto& entity : m_EntityManager.GetEntities()) {
    //        // So sánh địa chỉ con trỏ để xem thực thể của chúng ta có còn trong danh sách không
    //        if (entity.get() == m_player) {
    //            playerStillExists = true;
    //            break;
    //        }
    //    }

    //    if (!playerStillExists) {
    //        // Lấy sessionGold TRƯỚC KHI con trỏ m_player bị vô hiệu hóa
    //        int goldFromPickups = 0;
    //        if (auto* pc = m_player->TryGetComponent<PlayerControllerComponent>()) {
    //            goldFromPickups = pc->sessionGold;
    //        }

    //        // Nếu không tìm thấy, có nghĩa là người chơi đã bị hủy.
    //        // Đặt con trỏ thành nullptr để ngăn chặn mọi truy cập không an toàn sau này.
    //        m_player = nullptr;
    //        std::cout << "Player entity has been destroyed. GAME OVER!" << std::endl;
    //        // SỬA: Thay vì chỉ in ra, hãy chuyển sang trạng thái Game Over.
    //        int goldFromTime = static_cast<int>(m_playTime / 10);
    //        int totalGoldEarned = goldFromPickups + goldFromTime;

    //        SaveSystem::GetInstance()->AddGold(totalGoldEarned);
    //        SaveSystem::GetInstance()->SaveGame();

    //        Engine::GetInstance()->GetStateMachine()->ChangeState(new GameOverState(totalGoldEarned));
    //        return;
    //    }
    //}

    //// 2. Input
    //m_PlayerInputSystem.Update(m_EntityManager, deltaTime);

    //// 3. Spawner
    //m_EnemySpawnerSystem.Update(m_EntityManager, deltaTime);

    //// 4. AI
    //m_AISystem.Update(m_EntityManager, deltaTime);

    //// 5. Vũ khí
    ////m_WeaponSystem.Update(m_EntityManager, deltaTime);
    //m_WeaponControlSystem.Update(m_EntityManager, deltaTime); // Thêm dòng này

    //// 6. Di chuyển
    //m_EntityManager.Update(deltaTime);

    //// SỬA: Cập nhật camera SAU KHI người chơi đã di chuyển
    //if (m_player && m_pMap) {
    //    auto& playerTransform = m_player->GetComponent<TransformComponent>();
    //    Camera::GetInstance()->Update(playerTransform.position, m_pMap->GetWorldWidth(), m_pMap->GetWorldHeight());
    //}

    //// 7. MỚI: Cập nhật các thực thể "đi theo" (hào quang của tỏi)
    //// Phải chạy SAU khi người chơi đã di chuyển
    //m_FollowSystem.Update(m_EntityManager, deltaTime);

    //// 8. Va chạm
    //m_CollisionSystem.Update(m_EntityManager);

    //// 9. MỚI: Hệ thống xử lý sát thương Weapon-Enemy
    //m_DamageSystem.Update(m_EntityManager, deltaTime); // Thêm dòng này để xử lý sát thương từ vũ khí

    //// 10. Đạn
    //m_ProjectileSystem.Update(m_EntityManager, deltaTime);

    //// 11. Cập nhật các hitbox tạm thời
    //for (auto& entity : m_EntityManager.GetEntities()) {
    //    if (entity->HasComponent<HitboxComponent>()) {
    //        auto& hitbox = entity->GetComponent<HitboxComponent>();
    //        hitbox.timer += deltaTime;
    //        if (hitbox.timer >= hitbox.duration) {
    //            entity->Destroy();
    //        }
    //    }
    //}

    // // 13. Máu
    // m_HealthSystem.Update(m_EntityManager, deltaTime);

    //// MỚI: 14. Hệ thống nhặt đồ.
    //// Hút và nhặt các viên XP. Việc này có thể làm tăng XP của người chơi.
    //m_LootSystem.Update(m_EntityManager, deltaTime);

    //// MỚI: 15. Hệ thống lên cấp.
    //// Kiểm tra xem việc tăng XP có dẫn đến lên cấp hay không.
    //m_LevelSystem.Update(m_EntityManager, this, deltaTime);

    //// MỚI: 16. Thêm một vòng lặp ở cuối hàm Update để xử lý các hiệu ứng
    //for (auto& entity : m_EntityManager.GetEntities()) {
    //    if (entity->IsActive() && entity->HasComponent<DamageTextComponent>()) {
    //        auto& damageText = entity->GetComponent<DamageTextComponent>();
    //        damageText.timer += deltaTime;

    //        // Hủy thực thể nếu hết thời gian
    //        if (damageText.timer >= damageText.lifeTime) {
    //            entity->Destroy();
    //            continue; // Bỏ qua phần còn lại để tránh truy cập vào component đã hủy
    //        }

    //        // Cập nhật vị trí để bay lên
    //        if (entity->HasComponent<TransformComponent>()) {
    //            auto& transform = entity->GetComponent<TransformComponent>();
    //            transform.position.y -= damageText.floatSpeed * deltaTime;
    //        }

    //        // Cập nhật độ mờ (alpha) để mờ dần
    //        if (entity->HasComponent<TextComponent>()) {
    //            auto& text = entity->GetComponent<TextComponent>();
    //            float lifeRatio = damageText.timer / damageText.lifeTime;
    //            // Alpha sẽ giảm từ 255 xuống 0
    //            text.color.a = static_cast<Uint8>(255 * (1.0f - lifeRatio));
    //        }
    //    }
    //}

    //// 17. Cập nhật logic game khác (ví dụ: UI).
    //// Đoạn code này bây giờ hoàn toàn an toàn. Nếu m_player là nullptr, nó sẽ không chạy.
    //if (m_player && m_scoreUI) 
    //{
    //    auto& scoreComp = m_player->GetComponent<ScoreComponent>();
    //    auto& textComp = m_scoreUI->GetComponent<TextComponent>();
    //    std::string newScoreText = "Score: " + std::to_string(scoreComp.score);
    //    if (textComp.text != newScoreText) {
    //        textComp.text = newScoreText;
    //    }
    //}
}

void PlayState::Render() {
    // SỬA: Vẽ bản đồ trước tiên
    if (m_pMap) {
        m_pMap->Render(Engine::GetInstance()->GetRenderer());
    }

    m_RenderSystem.Render(m_EntityManager, Engine::GetInstance()->GetRenderer());
    m_TextRenderSystem.Render(m_EntityManager, Engine::GetInstance()->GetRenderer());

    // SỬA: Vẽ các chỉ số của người chơi lên trên cùng
    // Phải được gọi sau khi vẽ thế giới game nhưng trước khi vẽ UI nâng cấp
    m_StatsRenderSystem.Render(m_EntityManager, Engine::GetInstance()->GetRenderer());
    m_TimeRenderSystem.Render(m_playTime, Engine::GetInstance()->GetRenderer());

    // SỬA: Nếu game đang tạm dừng, vẽ thêm lớp UI nâng cấp lên trên
    if (m_isPausedForUpgrade) {
        RenderUpgradeUI();
    }
}

void PlayState::TriggerLevelUpScreen() {
    m_isPausedForUpgrade = true;
    m_currentUpgradeChoices = m_upgradeRegistry.GetRandomChoices(m_player, 3); // Lấy 3 lựa chọn ngẫu nhiên
    std::cout << "Game Paused! Choose an upgrade." << std::endl;
}

// SỬA ĐỔI HOÀN TOÀN HÀM NÀY
void PlayState::RenderUpgradeUI() {
    SDL_Renderer* renderer = Engine::GetInstance()->GetRenderer();
    AssetManager* assets = AssetManager::GetInstance(); // Lấy instance một lần để dùng nhiều lần

    // 1. Vẽ một lớp nền mờ để làm nổi bật UI
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); // Màu đen, độ mờ 150
    SDL_RenderFillRect(renderer, nullptr); // Vẽ lên toàn màn hình

    // 2. Vẽ các thẻ lựa chọn nâng cấp
    const int cardWidth = 300;
    const int cardHeight = 150;
    const int cardSpacing = 30;
    const int numCards = m_currentUpgradeChoices.size();
    const int totalWidth = (cardWidth * numCards) + (cardSpacing * (numCards - 1));

    int startX = (1280 - totalWidth) / 2; // Giả sử màn hình rộng 1280
    int startY = (720 - cardHeight) / 2;  // Giả sử màn hình cao 720

    SDL_Color titleColor = { 255, 215, 0, 255 }; // Màu vàng Gold
    SDL_Color descColor = { 220, 220, 220, 255 }; // Màu trắng xám

    for (size_t i = 0; i < numCards; ++i) {
        SDL_Rect cardRect = { startX + static_cast<int>(i) * (cardWidth + cardSpacing), startY, cardWidth, cardHeight };

        // Vẽ nền thẻ
        SDL_SetRenderDrawColor(renderer, 45, 52, 71, 255); // Màu xanh than đậm
        SDL_RenderFillRect(renderer, &cardRect);
        // Vẽ viền thẻ
        SDL_SetRenderDrawColor(renderer, 85, 99, 134, 255); // Màu xanh xám
        SDL_RenderDrawRect(renderer, &cardRect);

        // --- SỬA LỖI CHÍNH NẰM Ở ĐÂY ---
        // Chúng ta sẽ dùng quy trình mới: GetTextTexture -> Query -> RenderCopy

        // Vẽ Tên Nâng Cấp
        SDL_Texture* titleTex = assets->GetTextTexture(m_currentUpgradeChoices[i].name, "main_font", titleColor);
        if (titleTex) {
            int texW, texH;
            SDL_QueryTexture(titleTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect destRect = { cardRect.x + 15, cardRect.y + 15, texW, texH };
            SDL_RenderCopy(renderer, titleTex, nullptr, &destRect);
        }

        // Vẽ Mô Tả Nâng Cấp
        SDL_Texture* descTex = assets->GetTextTexture(m_currentUpgradeChoices[i].description, "main_font", descColor);
        if (descTex) {
            int texW, texH;
            SDL_QueryTexture(descTex, nullptr, nullptr, &texW, &texH);
            SDL_Rect destRect = { cardRect.x + 15, cardRect.y + 55, texW, texH }; // Dịch xuống dưới title
            SDL_RenderCopy(renderer, descTex, nullptr, &destRect);
        }
    }
}

void PlayState::HandleUpgradeSelection() {
    if (InputHandler::GetInstance()->IsMouseButtonPressed(1)) { // 1 là nút chuột trái
        Vector2D mousePos = InputHandler::GetInstance()->GetMousePosition();

        int cardWidth = 300;
        int cardHeight = 150;
        int startX = (1280 - (cardWidth * m_currentUpgradeChoices.size() + 20 * (m_currentUpgradeChoices.size() - 1))) / 2;
        int startY = (720 - cardHeight) / 2;

        for (size_t i = 0; i < m_currentUpgradeChoices.size(); ++i) {
            SDL_Rect cardRect = { startX + static_cast<int>(i) * (cardWidth + 20), startY, cardWidth, cardHeight };
            if (mousePos.x >= cardRect.x && mousePos.x <= cardRect.x + cardRect.w &&
                mousePos.y >= cardRect.y && mousePos.y <= cardRect.y + cardRect.h)
            {
                ApplyUpgradeAndResume(m_currentUpgradeChoices[i]);
                break; // Thoát khỏi vòng lặp sau khi đã chọn
            }
        }
    }
}

void PlayState::ApplyUpgradeAndResume(const UpgradeData& chosenUpgrade) {
    std::cout << "Selected: " << chosenUpgrade.name << std::endl;

    // Logic để thêm vũ khí mới
    if (chosenUpgrade.category == UpgradeData::Category::NEW_WEAPON) {
        if (chosenUpgrade.targetStat == "magic_wand") m_player->AddComponent<MagicWandComponent>();
        else if (chosenUpgrade.targetStat == "whip") m_player->AddComponent<WhipComponent>();
        else if (chosenUpgrade.targetStat == "garlic") m_player->AddComponent<GarlicAuraComponent>();
    }
    // Logic để áp dụng nâng cấp chỉ số
    else if (m_player && m_player->HasComponent<StatsComponent>()) {
        auto& stats = m_player->GetComponent<StatsComponent>();

        if (chosenUpgrade.isMultiplier) { // Nâng cấp dạng nhân (%)
            if (chosenUpgrade.targetStat == "damageMultiplier") stats.damageMultiplier *= chosenUpgrade.value;
            else if (chosenUpgrade.targetStat == "cooldownReduction") stats.cooldownReduction *= chosenUpgrade.value;
            else if (chosenUpgrade.targetStat == "areaSizeMultiplier") stats.areaSizeMultiplier *= chosenUpgrade.value;
            else if (chosenUpgrade.targetStat == "projectileSpeedMultiplier") stats.projectileSpeedMultiplier *= chosenUpgrade.value;
            else if (chosenUpgrade.targetStat == "maxHealthMultiplier") {
                stats.maxHealthMultiplier *= chosenUpgrade.value;
                auto& health = m_player->GetComponent<HealthComponent>();
                health.maxHealth = stats.GetCurrentMaxHealth();
                health.currentHealth = health.maxHealth; // Hồi đầy máu
            }
        }
        else { // Nâng cấp dạng cộng (+)
            if (chosenUpgrade.targetStat == "projectileCountBonus") {
                stats.projectileCountBonus += static_cast<int>(chosenUpgrade.value);
            }
        }
    }

    // Tiếp tục game
    m_isPausedForUpgrade = false;
    m_currentUpgradeChoices.clear();
    std::cout << "Game Resumed!" << std::endl;
}
