#include "Engine.h"
#include "SaveSystem.h"
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

// Global function for Emscripten main loop
void emscripten_main_loop() {
    Engine::GetInstance()->Run();
}

// Cleanup function for Emscripten
void emscripten_cleanup() {
    Engine::GetInstance()->CleanUp();
}
#endif

int main(int argc, char* args[]) {
    // Load dữ liệu game đã lưu trước khi khởi tạo bất cứ thứ gì khác
    SaveSystem::GetInstance()->LoadGame();

    if (!Engine::GetInstance()->Init("SDL2 Roguelike Survivor", 1280, 720)) {
        std::cerr << "--- Game failed to initialize. ---" << std::endl;
        return -1;
    }

#ifdef __EMSCRIPTEN__
    // Web: Use Emscripten's main loop
    std::cout << "Starting Emscripten main loop..." << std::endl;
    
    // Register cleanup function
    atexit(emscripten_cleanup);
    
    // Start the main loop (60 FPS, simulate infinite loop)
    emscripten_set_main_loop(emscripten_main_loop, 60, 1);
#else
    // Desktop: Run traditional game loop
    std::cout << "Starting desktop game loop..." << std::endl;
    Engine::GetInstance()->Run();
    
    // Dọn dẹp
    Engine::GetInstance()->CleanUp();
#endif

    return 0;
}