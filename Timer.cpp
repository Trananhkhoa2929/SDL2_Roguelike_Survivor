#include "Timer.h"

Timer* Timer::s_Instance = nullptr;

void Timer::Tick() 
{	
	// lấy thời gian hiện tại bằng bộ đếm hiệu suất cao của SDL
	Uint64 currentTime = SDL_GetPerformanceCounter();

	// nếu là tick đầu tien
	// SỬA: Logic cũ có thể gây ra deltaTime = 0 ở frame đầu tiên trong vòng lặp.
	// Logic mới đảm bảo m_LastTime luôn hợp lệ.
	if (m_LastTime == 0) {
		m_LastTime = currentTime;
	}
	// (Thời gian hiện tại - Thời gian trước) / Tần số của bộ đếm = Delta Time (giây)
	m_DeltaTime = static_cast<float>(currentTime - m_LastTime) /
				static_cast<float>(SDL_GetPerformanceFrequency());

	m_LastTime = currentTime;

	if (m_DeltaTime > 0.05f) {
		m_DeltaTime = 0.05f;
	}
}