#include "UILabel.h"

#include "graphics/Vertex.h"
#include "util/AssetCache.h"

void UILabel::FromJSON(const json& j) {
	_text = j["text"];
	_fontPath = j["font"];
	_fontSize = j["fontSize"];
	if (j.contains("color")) {
		_color = glm::vec4(
			j["color"][0],
			j["color"][1],
			j["color"][2],
			j["color"][3]
		);
	} else {
		_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	_hAlign = j.contains("hAlign") ? AlignMap.strToEnum.at(j["hAlign"]) : Align::Start;
}

void UILabel::OnCreate(std::weak_ptr<Scene> scene) {
	UIWidget::OnCreate(scene);
	_font = AssetCache<Font>::Instance().Get(_fontPath);
	_material->Init();
	_material->SetAtlas(&_font->GetAtlasTexture());
	_material->SetColor(_color);
	_mesh = Mesh({}, {}, Mesh::DrawHint::Dynamic);
}

void UILabel::_Arrange() {
	_meshDirty = true;
}

glm::vec2 UILabel::MeasureContent() {
	if (!_font) {
		return glm::vec2(0, 0);
	}
	return _font->MeasureText(_text, _fontSize);
}

void UILabel::Draw(Renderer& renderer) {
	if (_meshDirty) {
		_BuildMesh();
	}
	renderer.SubmitMesh(_mesh, _material, glm::mat4(1.0f),
		[]() { glDisable(GL_DEPTH_TEST); },
		[]() { glEnable(GL_DEPTH_TEST); }
	);
}

void UILabel::_BuildMesh() {
	const Rect& rect = _layout->GetComputedRect();
	glm::vec2 measured = _font->MeasureText(_text, _fontSize);
	
	float startX;
	switch (_hAlign) {
		case Align::Start: startX = rect.x; break;
		case Align::Center: startX = rect.x + (rect.width - measured.x) / 2.0f; break;
		case Align::End: startX = rect.x + rect.width - measured.x; break;
	}
	float baseY = rect.y;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	float cursor = startX;

	for (char c : _text) {
		const Font::Glyph& g = _font->GetGlyph(c);
		if (!g.hasGeometry) {
			cursor += g.advance * _fontSize;
			continue;
		}

		unsigned int i = vertices.size();
		vertices.push_back({{ cursor + g.planeLeft * _fontSize, baseY - _fontSize + g.planeTop * _fontSize,    0 }, { 0, 0, 1 }, { g.uvLeft,  g.uvBottom }});
        vertices.push_back({{ cursor + g.planeRight * _fontSize, baseY - _fontSize + g.planeTop * _fontSize,   0 }, { 0, 0, 1 }, { g.uvRight, g.uvBottom }});
        vertices.push_back({{ cursor + g.planeLeft * _fontSize, baseY - _fontSize + g.planeBottom * _fontSize, 0 }, { 0, 0, 1 }, { g.uvLeft,  g.uvTop    }});
        vertices.push_back({{ cursor + g.planeRight * _fontSize, baseY - _fontSize + g.planeBottom * _fontSize,0 }, { 0, 0, 1 }, { g.uvRight, g.uvTop    }});
		indices.insert(indices.end(), { i, i+2, i+1, i+1, i+2, i+3 });
		cursor += g.advance * _fontSize;
	}
	_mesh.Upload(vertices, indices);
	_meshDirty = false;
}

REGISTER_PROPERTY(UILabel)
