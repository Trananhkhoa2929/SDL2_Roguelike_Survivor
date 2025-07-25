#pragma once
#include "IGameState.h"
#include <SDL.h>

class PauseState : public IGameState {
public:
    virtual void OnEnter() override;
    virtual void OnExit() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

private:
    void HandleInput();
    SDL_Rect m_continueButtonRect;
    SDL_Rect m_mainMenuButtonRect;
};