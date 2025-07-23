#include "InputHandler.h"
#include <cstring>

InputHandler* InputHandler::s_Instance = nullptr;

InputHandler::InputHandler() : m_MouseState(0), m_MouseX(0), m_MouseY(0) // BUG FIX: Khởi tạo các biến thành viên
{
	// trỏ tới mảng trạng thái phím của SDL
	m_Keystates = SDL_GetKeyboardState(&m_KeyCount);
	// tạo bản sao cho trạng thái frame trước
	m_PrevKeystates = new Uint8[m_KeyCount];
	memcpy(m_PrevKeystates, m_Keystates, m_KeyCount);
}

InputHandler::~InputHandler() {
	delete m_PrevKeystates;
	m_PrevKeystates = nullptr;
}

// REFACTOR: Thêm hàm Clean để có thể gọi từ bên ngoài
void InputHandler::Clean() {
	delete s_Instance;
	s_Instance = nullptr;
}

void InputHandler::Listen() {
	SDL_Event event;
	
	// Sao chép trạng thái phím của frame trước đó.
	// Điều này rất quan trọng để phát hiện IsKeyPressed và IsKeyReleased.
	memcpy(m_PrevKeystates, m_Keystates, m_KeyCount);

	// BUG FIX: Cập nhật trạng thái chuột ở đầu mỗi frame.
	// SDL_GetMouseState là cách hiệu quả để lấy trạng thái hiện tại.
	m_PrevMouseState = m_MouseState;

	m_MouseState = SDL_GetMouseState(&m_MouseX, &m_MouseY);
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_QuitRequested = true;
			break;

			// BUG FIX: Xử lý sự kiện bàn phím để SDL_GetKeyboardState hoạt động chính xác.
			// Mặc dù chúng ta lấy trạng thái bằng m_Keystates, việc poll event vẫn cần thiết.
		case SDL_KEYDOWN:
			m_Keystates = SDL_GetKeyboardState(NULL);
			break;
		case SDL_KEYUP:
			m_Keystates = SDL_GetKeyboardState(NULL);
			break;

			// NOTE: Các sự kiện chuột cũng có thể được xử lý ở đây nếu cần logic
			// phức tạp hơn (ví dụ: double click), nhưng đối với IsMouseButtonDown,
			// việc gọi SDL_GetMouseState ở trên là đủ.
		}
	}
}

bool InputHandler::IsKeyDown(SDL_Scancode key) const {
	return (m_Keystates[key] == 1);
}

bool InputHandler::IsKeyPressed(SDL_Scancode key) const {
	return (m_Keystates[key] == 1 && m_PrevKeystates[key] == 0);
}

bool InputHandler::IsKeyReleased(SDL_Scancode key) const {
	return (m_Keystates[key] == 0 && m_PrevKeystates[key] == 1);
}

bool InputHandler::IsMouseButtonDown(int button) const {
	// BUG FIX: Hàm này giờ sẽ hoạt động chính xác vì m_MouseState đã được cập nhật.
	return (m_MouseState & SDL_BUTTON(button));
}

// Hàm mới
bool InputHandler::IsMouseButtonPressed(int button) const {
	return (m_MouseState & SDL_BUTTON(button)) && !(m_PrevMouseState & SDL_BUTTON(button));
}

Vector2D InputHandler::GetMousePosition() const {
	return Vector2D(static_cast<float>(m_MouseX), static_cast<float>(m_MouseY));
}