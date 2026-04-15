#pragma once

#include "game/Properties/2D/UI/UIWidget.h"
#include "graphics/Font.h"
#include "graphics/Mesh.h"
#include "graphics/materials/MSDFMaterial.h"

class UILabel : public UIWidget {
public:
	void FromJSON(const json &j) override;
	void Draw(Renderer &renderer) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	glm::vec2 MeasureContent() override;
	void SetText(const std::string& text);
	void SetTextColor(const glm::vec4& color);
	glm::vec2 MeasureText(const std::string& text) const;

private:
	void _Arrange() override;
	void _BuildMesh();

	std::string _fontPath;
	std::shared_ptr<Font> _font;
	std::string _text;
	float _fontSize;
	Align _hAlign;
	glm::vec4 _color;
	Mesh _mesh;
	std::shared_ptr<MSDFMaterial> _material = std::make_shared<MSDFMaterial>();
	bool _meshDirty = true;
};
