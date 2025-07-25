// File: src/States/UpgradeShopState.h (MỚI)
#pragma once
#include "IGameState.h"
#include <vector>
#include <string>
#include <functional>
#include <SDL.h>

// MỚI: Struct để quản lý thông tin một nâng cấp vĩnh viễn trong cửa hàng
struct PermanentUpgrade {
    std::string id;
    std::string name;
    std::string description;
    int maxLevel;
    // Hàm để tính giá tiền cho cấp độ tiếp theo
    std::function<int(int currentLevel)> costFunction;
    SDL_Rect buyButtonRect;
};

class UpgradeShopState : public IGameState {
public:
    virtual void OnEnter() override;
    virtual void OnExit() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

private:
    void HandleInput();
    void PopulateUpgrades();
    void RenderGoldUI(SDL_Renderer* renderer);

    std::vector<PermanentUpgrade> m_upgrades;
    SDL_Rect m_backButtonRect;
};