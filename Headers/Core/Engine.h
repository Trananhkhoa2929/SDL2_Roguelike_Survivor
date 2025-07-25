#pragma once

#include <SDL.h>
#include <memory>
#include "GameStateMachine.h"

class Engine
{
public:
	static Engine* GetInstance() {
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new Engine();
	}

	bool Init(const char* title, int width, int height);

	void Run();
	void CleanUp();
	void Quit();

	// Hàm để các hệ thống khác có thể lấy renderer
	SDL_Renderer* GetRenderer() const { return m_Renderer; }

	// SỬA: Thêm hàm public này để các trạng thái khác (như MainMenuState)
	// có thể truy cập vào State Machine và thay đổi trạng thái game.
	GameStateMachine* GetStateMachine() const { return m_GameStateMachine.get(); }

private:
	Engine() {}
	~Engine() {}

	void Update();
	void Render();

	// TỐI ƯU: Sử dụng std::unique_ptr để tự động dọn dẹp bộ nhớ cho state machine
	std::unique_ptr<GameStateMachine> m_GameStateMachine;

	bool m_IsRunning = false;
	SDL_Window* m_Window = nullptr;
	SDL_Renderer* m_Renderer;

	static Engine* s_Instance;
};