#include "Engine.h"
#include "Timer.h"
#include "InputHandler.h"
#include "AssetManager.h"
#include "PlayState.h"
#include "MainMenuState.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

Engine* Engine::s_Instance = nullptr;

#ifdef __EMSCRIPTEN__
// Global pointer để emscripten_set_main_loop có thể truy cập
Engine* g_engine = nullptr;

// Hàm callback cho main loop của Emscripten
void main_loop() {
	if (g_engine && g_engine->IsRunning()) {
		g_engine->RunSingleFrame();
	}
}
#endif

bool Engine::Init(const char* title, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
		std::cerr << "Failed to initialize SDL_image: " << SDL_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
		return false;
	}

	m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (m_Window == nullptr) {
		std::cerr << "Failed to create Window: " << SDL_GetError() << std::endl;
		return false;
	}

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) {
		std::cerr << "Failed to create Renderer: " << SDL_GetError() << std::endl;
		return false;
	}

	AssetManager::GetInstance()->Init(m_Renderer);
	InputHandler::GetInstance();

	m_GameStateMachine = std::make_unique<GameStateMachine>();
	m_GameStateMachine->ChangeState(new MainMenuState());

	std::cout << "Engine initialization successful!" << std::endl;
	m_IsRunning = true;

	return true;
}

void Engine::Run()
{
#ifdef __EMSCRIPTEN__
	g_engine = this;
	// Sử dụng 0 cho fps (sẽ sync với browser's requestAnimationFrame)
	// simulate_infinite_loop = 1 để emscripten biết đây là main loop
	emscripten_set_main_loop(main_loop, 0, 1);
#else
	// Desktop: Traditional game loop
	while (m_IsRunning) {
		RunSingleFrame();
	}
#endif
}

// Tách logic một frame thành hàm riêng để dùng chung
void Engine::RunSingleFrame()
{
	Timer::GetInstance()->Tick();
	InputHandler::GetInstance()->Listen();

	if (InputHandler::GetInstance()->IsQuitRequested()) {
		m_IsRunning = false;
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#endif
		return;
	}

	Update();
	Render();
}

void Engine::Update()
{
	m_GameStateMachine->Update(Timer::GetInstance()->GetDeltaTime());
}

void Engine::Render()
{
	SDL_RenderClear(m_Renderer);
	m_GameStateMachine->Render();
	SDL_RenderPresent(m_Renderer);
}

void Engine::CleanUp()
{
	std::cout << "Start cleaning up engine..." << std::endl;

	if (m_GameStateMachine) {
		m_GameStateMachine->Clean();
	}

	AssetManager::GetInstance()->Clean();

	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Engine::Quit()
{
	m_IsRunning = false;
#ifdef __EMSCRIPTEN__
	emscripten_cancel_main_loop();
#endif
}

// Thêm getter để kiểm tra trạng thái
bool Engine::IsRunning() const
{
	return m_IsRunning;
}