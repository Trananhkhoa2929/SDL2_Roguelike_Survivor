#pragma once

#include "IGameState.h"
#include <SDL.h> // Cần cho SDL_Rect
#include "SaveSystem.h"

class GameOverState : public IGameState {
public:
    // SỬA: Thêm constructor để nhận vàng
    GameOverState(int goldEarned);

    virtual void OnEnter() override;
    virtual void OnExit() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

private:
    void HandleInput();

    // Lưu lại vùng chữ nhật của các nút để xử lý click
    SDL_Rect m_replayButtonRect;
    SDL_Rect m_exitButtonRect;

    int m_goldEarned;
};