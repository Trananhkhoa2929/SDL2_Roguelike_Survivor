#include "Engine.h"
#include "SaveSystem.h"
#include <iostream>

int main(int argc, char* args[]) {
    // Load dữ liệu game đã lưu trước khi khởi tạy bất cứ thứ gì khác
    SaveSystem::GetInstance()->LoadGame();

    if (!Engine::GetInstance()->Init("Noobs Are Coming Clone", 1280, 720)) {
        std::cerr << "--- Game failed to initialize. ---" << std::endl;
        return -1;
    }

    // Chạy vòng lặp game
    Engine::GetInstance()->Run();

    // Dọn dẹp
    Engine::GetInstance()->CleanUp();

    return 0;
}