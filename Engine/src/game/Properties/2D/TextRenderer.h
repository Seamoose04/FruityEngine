#pragma once

#include "game/Properties/3D/Transform.h"
#include "game/Property.h"
#include "game/PropertyRef.h"
#include "graphics/Font.h"
#include "graphics/Mesh.h"
#include "graphics/materials/MSDFMaterial.h"
#include <memory>
#include <string>

class TextRenderer : public Property {
public:
	void FromJSON(const json &data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Render(Renderer &renderer) override;
	void SetText(const std::string& text);

private:
	void _BuildMesh();

	PropertyRef<Transform> _transform;
	std::shared_ptr<Font> _font;
	std::shared_ptr<MSDFMaterial> _material = std::make_shared<MSDFMaterial>();
	Mesh _mesh;
	std::string _text;
	std::string _fontPath;
	float _fontSize;
	glm::vec4 _color;
};
