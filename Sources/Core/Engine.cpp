#include "Engine.h"
#include "Timer.h"
#include "InputHandler.h"
#include "AssetManager.h"
#include "PlayState.h"
#include "MainMenuState.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

Engine* Engine::s_Instance = nullptr;
// SỬA: Không cần định nghĩa biến static ở đây nữa vì nó đã là thành viên bình thường


bool Engine::Init(const char* title, int width, int height) 
{
	// 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return false;
	}
	//
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
		std::cerr << "Failed to initialize SDL_image: " << SDL_GetError() << std::endl;
		return false;
	}
	// SỬA: Thêm TTF_Init()
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
		return false;
	}
	// tạo window và renderer
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
	// khởi tạo các lớp singleton
	// SỬA: PHẦN QUAN TRỌNG NHẤT
	// Chúng ta phải gọi Init() cho AssetManager và truyền Renderer vào.
	AssetManager::GetInstance()->Init(m_Renderer);
	// InputHandler không cần Init phức tạp nên GetInstance() là đủ.
	InputHandler::GetInstance();
	
	// Khởi tạo state machine
	// TỐI ƯU: Dùng std::make_unique để an toàn hơn
	m_GameStateMachine = std::make_unique<GameStateMachine>();
	// SỬA: Phải khởi tạo state machine SAU KHI các manager đã sẵn sàng
	m_GameStateMachine->ChangeState(new MainMenuState()); // <-- THAY ĐỔI Ở ĐÂY
	//m_GameStateMachine->ChangeState(new PlayState());

	std::cout << "Engine initialization successful!" << std::endl;
	m_IsRunning = true;


	// SỬA: XÓA BỎ VIỆC LOAD ASSET Ở ĐÂY.
	// Engine không nên biết về các asset cụ thể của game.
	// Việc load asset thuộc về trách nhiệm của các Game State (ví dụ PlayState::OnEnter).
	// AssetManager::GetInstance()->LoadTexture("player", "D:/Asset/player.png"); // <--- XÓA DÒNG NÀY

	return true;
}

void Engine::Run() 
{
	while (m_IsRunning) {
		// SỬA: Timer::Tick() chỉ nên được gọi MỘT LẦN ở đầu mỗi frame.
		Timer::GetInstance()->Tick();

		InputHandler::GetInstance()->Listen();
		if (InputHandler::GetInstance()->IsQuitRequested()) {
			m_IsRunning = false;
		}

		Update();
		Render();
	}
}


void Engine::Update()
{
	// SỬA: XÓA BỎ LỜI GỌI Tick() Ở ĐÂY.
	// Timer::GetInstance()->Tick();

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

	// TỐI ƯU: unique_ptr sẽ tự dọn dẹp state machine, nhưng gọi Clean() vẫn cần thiết
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
}