#include "TextRenderer.h"

#include "game/Property.h"
#include "graphics/Renderer.h"
#include "util/AssetCache.h"

void TextRenderer::FromJSON(const json& j) {
	_text = j["text"];
	_fontPath = j["font"];
	_fontSize = j["fontSize"];
	_color = glm::vec4(j["color"][0], j["color"][1], j["color"][2], j["color"][3]);
}

void TextRenderer::OnCreate(std::weak_ptr<Scene> scene) {
	_transform.From(_gameObject);
	_font = AssetCache<Font>::Instance().Get(_fontPath);
	_material->Init();
	_material->SetAtlas(&_font->GetAtlasTexture());
	_material->SetColor(_color);
	_mesh = Mesh({}, {}, Mesh::DrawHint::Dynamic);
	_BuildMesh();
}

void TextRenderer::SetText(const std::string& text) {
	_text = text;
	_BuildMesh();
}

void TextRenderer::_BuildMesh() {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	float cursor = 0.0f;

	for (char c : _text) {
		const Font::Glyph& g = _font->GetGlyph(c);
		if (!g.hasGeometry) {
			cursor += g.advance * _fontSize;
			continue;
		}
		

		unsigned int i = vertices.size();

		vertices.push_back({{ cursor + g.planeLeft * _fontSize, g.planeTop * _fontSize, 0 }, { 0, 0, 1 }, { g.uvLeft, g.uvBottom }});
		vertices.push_back({{ cursor + g.planeRight * _fontSize, g.planeTop * _fontSize, 0 }, { 0, 0, 1 }, { g.uvRight, g.uvBottom }});
		vertices.push_back({{ cursor + g.planeLeft * _fontSize, g.planeBottom * _fontSize, 0 }, { 0, 0, 1 }, { g.uvLeft, g.uvTop }});
		vertices.push_back({{ cursor + g.planeRight * _fontSize, g.planeBottom * _fontSize, 0 }, { 0, 0, 1 }, { g.uvRight, g.uvTop }});

		indices.insert(indices.end(), { i, i + 2, i + 1, i + 1, i + 2, i + 3 });

		cursor += g.advance * _fontSize;
	}

	_mesh.Upload(vertices, indices);
}

void TextRenderer::Render(Renderer& renderer) {
	renderer.SubmitMesh(_mesh, _material, _transform->GetWorldMatrix());
}

REGISTER_PROPERTY(TextRenderer)
