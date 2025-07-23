#include "AssetManager.h"
#include "Engine.h" // lấy renderer
#include <SDL_ttf.h>
#include <iostream>

AssetManager* AssetManager::s_Instance = nullptr;

// REFACTOR: Hàm Init để nhận renderer
void AssetManager::Init(SDL_Renderer* renderer) {
	m_Renderer = renderer;
}

bool AssetManager::LoadTexture(const std::string& id, const std::string& filepath)
{
	// REFACTOR: Sử dụng m_Renderer đã được lưu trữ
	if (m_Renderer == nullptr) {
		std::cerr << "AssetManager not initialized! Call Init() first." << std::endl;
		return false;
	}

	// load texture fromfile
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	if (surface == nullptr) {
		std::cerr << "Failed to load surface from texture " << id << ": " << IMG_GetError() << std::endl;
		return false;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
	SDL_FreeSurface(surface);
	if (texture == nullptr) {
		std::cerr << "Failed to create texture from surface " << id << ": " << SDL_GetError() << std::endl;
		return false;
	}
	m_TextureMap[id] = texture;
	std::cout << "Successfully loaded texture: " << id << std::endl;
	return true;

}

SDL_Texture* AssetManager::GetTexture(const std::string& id) {
	return (m_TextureMap.count(id)) ? m_TextureMap[id] : nullptr;
}

bool AssetManager::LoadFont(const std::string& id, const std::string& filepath, int size) {
	TTF_Font* font = TTF_OpenFont(filepath.c_str(), size);
	if (font == nullptr) {
		std::cerr << "Failed to load font " << id << ": " << TTF_GetError() << std::endl;
		return false;
	}
	m_FontMap[id] = font;
	return true;
}

// PERFORMANCE REFACTOR: Thay thế hoàn toàn RenderText
SDL_Texture* AssetManager::GetTextTexture(const std::string& text, const std::string& fontId, SDL_Color color)
{
	if (m_FontMap.find(fontId) == m_FontMap.end()) {
		std::cerr << "Font not found: " << fontId << std::endl;
		return nullptr;
	}
	if (m_Renderer == nullptr) {
		std::cerr << "AssetManager not initialized for text rendering!" << std::endl;
		return nullptr;
	}
	// 1. Tạo một key duy nhất cho cache
	std::string cacheKey = fontId + "_" + text + "_" + std::to_string(color.r) + std::to_string(color.g) + std::to_string(color.b);

	// 2. Kiểm tra xem texture đã có trong cache chưa
	if (m_TextTextureCache.count(cacheKey)) {
		return m_TextTextureCache[cacheKey];
	}

	// 3. Nếu chưa có, tạo texture mới
	// SỬA LỖI FONT: Dùng TTF_RenderUTF8_Blended để hỗ trợ tiếng Việt
	SDL_Surface* surface = TTF_RenderUTF8_Blended(m_FontMap[fontId], text.c_str(), color);
	if (!surface) {
		std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture) {
		std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	// 4. Lưu texture mới vào cache và trả về nó
	m_TextTextureCache[cacheKey] = texture;
	return texture;
}

TTF_Font* AssetManager::GetFont(const std::string& id) {
	return (m_FontMap.count(id)) ? m_FontMap[id] : nullptr;
}

void AssetManager::Clean() 
{
	for (auto const& [id, texture] : m_TextureMap) {
		SDL_DestroyTexture(texture);
	}
	m_TextureMap.clear();

	// PERFORMANCE: Đừng quên dọn dẹp cache của text
	for (auto const& [key, texture] : m_TextTextureCache) {
		SDL_DestroyTexture(texture);
	}
	m_TextTextureCache.clear();

	for (auto const& [id, font] : m_FontMap) {
		TTF_CloseFont(font);
	}
	m_FontMap.clear();

	std::cout << "Asset Manager cleaned." << std::endl;

	// REFACTOR: Giải phóng singleton instance
	delete s_Instance;
	s_Instance = nullptr;

}