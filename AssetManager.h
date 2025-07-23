#pragma once
#include "SDL_image.h" // load image nhiều kiểu (png, ...)
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include <map>

class AssetManager
{
public:
	// singleton 
	static AssetManager* GetInstance() {
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new AssetManager();
	}

	// REFACTOR: Thêm hàm Init để truyền vào renderer (Dependency Injection)
	void Init(SDL_Renderer* renderer);

	// load a texture from file and store it by identified ID
	bool LoadTexture(const std::string& id, const std::string& filepath);
	// lấy một texture đã được load bằng id
	SDL_Texture* GetTexture(const std::string& id);

	bool LoadFont(const std::string& id, const std::string& filepath, int size);
	// REFACTOR: Thay đổi hàm RenderText thành GetTextTexture để tối ưu hóa.
		// Hệ thống render sẽ chịu trách nhiệm vẽ texture này.
	SDL_Texture* GetTextTexture(const std::string& text, const std::string& fontId, SDL_Color color);
	// để xuống dòng khi hết không gian
	TTF_Font* GetFont(const std::string& id); 
	void Clean();

private:
	AssetManager() {}
	~AssetManager() {}

	// REFACTOR: Lưu trữ con trỏ renderer
	SDL_Renderer* m_Renderer = nullptr;

	std::map<std::string, SDL_Texture*> m_TextureMap;
	std::map<std::string, TTF_Font*> m_FontMap;

	// PERFORMANCE: Thêm một map để cache các texture của text.
	// Key là sự kết hợp của fontId, text và màu sắc để đảm bảo tính duy nhất.
	std::map<std::string, SDL_Texture*> m_TextTextureCache;

	static AssetManager* s_Instance;
};