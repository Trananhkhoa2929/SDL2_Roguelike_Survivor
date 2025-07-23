#pragma once

#include "SDL.h"
#include <vector>
#include "Vector2D.h"

class InputHandler
{
public:
	static InputHandler* GetInstance() {
		// NOTE: Singleton pattern đơn giản. Đảm bảo nó được dọn dẹp đúng cách
		// ở cuối chương trình để tránh rò rỉ bộ nhớ.

		return s_Instance = (s_Instance != nullptr) ? s_Instance : new InputHandler();
	}
	
	void Listen();
	void Clean(); // REFACTOR: Thêm hàm Clean để giải phóng bộ nhớ của singleton

	bool IsMouseButtonDown(int button) const;
	bool IsMouseButtonPressed(int button) const;
	Vector2D GetMousePosition() const;
	bool IsKeyDown(SDL_Scancode key) const;
	bool IsKeyPressed(SDL_Scancode key) const;
	bool IsKeyReleased(SDL_Scancode key) const;
	bool IsQuitRequested() const { return m_QuitRequested; }

private:
	InputHandler();
	~InputHandler();
	// con trỏ tới mảng trạng thái nội bộ của SDL
	const Uint8* m_Keystates;
	// mảng trạng thái phím của frame trước đó (để so sánh)
	Uint8* m_PrevKeystates;
	int m_KeyCount;

	// BUG FIX: m_MouseState, m_MouseX, m_MouseY chưa bao giờ được cập nhật.
	// Chúng sẽ được cập nhật trong hàm Listen().
	Uint32 m_MouseState;
	Uint32 m_PrevMouseState;
	int m_MouseX, m_MouseY;

	bool m_QuitRequested = false;
	static InputHandler* s_Instance;
};