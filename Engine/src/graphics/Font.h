#pragma once

#include <json.hpp>
#include <memory>
#include "Texture.h"

class Font {
public:
	struct Glyph {
		float advance;
		float planeLeft, planeBottom, planeRight, planeTop;
		float uvLeft, uvBottom, uvRight, uvTop;
		bool hasGeometry;
	};

	Font() = default;
	Font(const std::string& imagePath, const std::string& jsonPath);

	bool Load(const std::string& imagePath, const std::string& jsonPath);
	
	const Glyph& GetGlyph(char c) const;
	Texture& GetAtlasTexture();
	glm::vec2 MeasureText(const std::string& text, float fontSize) const;
	
	static std::shared_ptr<Font> Load(const std::string& path);

private:
	Texture _atlas;
	std::unordered_map<uint32_t, Glyph> _glyphs;
	float _atlasWidth, _atlasHeight;
	float _size;
};
