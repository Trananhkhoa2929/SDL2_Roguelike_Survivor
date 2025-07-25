#pragma once

#include <SDL.h>

class Timer
{
public:
	// vì time là duy nhất nên thiết kế kiểu singleton
	static Timer* GetInstance() {
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new Timer();
	}

	// được gọi một lần mỗi frame để cập nhật delta time
	void Tick();

	inline float GetDeltaTime() const { return m_DeltaTime; }

private:
	Timer() {}
	~Timer() {}
	
	static Timer* s_Instance;
	
	float m_DeltaTime = 0.0f;
	Uint64 m_LastTime = 0;

};