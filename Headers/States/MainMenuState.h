#pragma once
#include "IGameState.h"
#include "UpgradeSystem.h"
#include <vector>
#include <SDL.h>

class MainMenuState : public IGameState
{
public:
	virtual void OnEnter() override;
	virtual void OnExit() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

private:
    void HandleSelection();

    // Lưu trữ các lựa chọn vũ khí khởi đầu để hiển thị
    std::vector<UpgradeData> m_startingWeaponChoices;
    std::vector<SDL_Rect> m_weaponCardRects;
    SDL_Rect m_upgradeButtonRect;
};
