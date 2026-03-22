#include "Font.h"

#include "core/FileLoaders/JSONLoader.h"
#include <cstdlib>
#include <iostream>
#include <memory>

Font::Font(const std::string& imagePath, const std::string& jsonPath) {
	if (!Load(imagePath, jsonPath)) {
		std::abort();
	}
}

bool Font::Load(const std::string& imagePath, const std::string& jsonPath) {
	if (!_atlas.Load(imagePath)) {
		return false;
	}
	JSONLoader loader;
	json j = loader.LoadJSON(jsonPath);
	
	json& atlasData = j["atlas"];
	_atlasWidth = atlasData["width"];
	_atlasHeight = atlasData["height"];
	_size = atlasData["size"];

	for (const auto& g : j["glyphs"]) {
		Glyph glyph;
		glyph.advance = g["advance"];

		if (g.contains("planeBounds")) {
			glyph.hasGeometry = true;
			glyph.planeLeft = g["planeBounds"]["left"];
			glyph.planeBottom = g["planeBounds"]["bottom"];
			glyph.planeRight = g["planeBounds"]["right"];
			glyph.planeTop = g["planeBounds"]["top"];

			glyph.uvLeft = (float)g["atlasBounds"]["left"] / _atlasWidth;
			glyph.uvRight = (float)g["atlasBounds"]["right"] / _atlasWidth;
			glyph.uvBottom = 1.0f - ((float)g["atlasBounds"]["top"] / _atlasHeight);
			glyph.uvTop = 1.0f - ((float)g["atlasBounds"]["bottom"] / _atlasHeight);
		} else {
			glyph.hasGeometry = false;
		}

		_glyphs[g["unicode"]] = glyph;
	}

	return true;
}

std::shared_ptr<Font> Font::Load(const std::string& path) {
	auto font = std::make_shared<Font>();
	font->Load(path + ".png", path + ".json");
	return font;
}

Texture& Font::GetAtlasTexture() {
	return _atlas;
}

const Font::Glyph& Font::GetGlyph(char c) const {
	auto it = _glyphs.find((uint32_t)c);
	if (it == _glyphs.end()) {
		std::cerr << "[Font] Glyph not found for character: " << c << std::endl;
		std::abort();
	}
	return it->second;
}
