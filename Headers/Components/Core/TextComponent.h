#pragma once
#include "IComponent.h"
#include <string>
#include <SDL.h>

class TextComponent : public IComponent
{
public:
	std::string text;
	std::string fontId;	
	SDL_Color color;

	// MỚI: Cờ để xác định text có bị ảnh hưởng bởi camera hay không
	bool isFixedToScreen = false;

	TextComponent(const std::string& txt, const std::string& font, const SDL_Color& c, bool fixed = false) :
		text(txt), fontId(font), color(c), isFixedToScreen(fixed) {}

};