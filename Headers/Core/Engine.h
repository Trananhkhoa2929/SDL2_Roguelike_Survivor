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

	// SỬA: Thêm hàm public này để các trạng thái khác có thể truy cập
	GameStateMachine* GetStateMachine() const { return m_GameStateMachine.get(); }

	// MỚI: Thêm các hàm cần thiết cho Emscripten
	void RunSingleFrame();
	bool IsRunning() const;

private:
	Engine() {}
	~Engine() {}

	void Update();
	void Render();

	std::unique_ptr<GameStateMachine> m_GameStateMachine;

	bool m_IsRunning = false;
	SDL_Window* m_Window = nullptr;
	SDL_Renderer* m_Renderer;

	static Engine* s_Instance;
};