#include "UIInput.h"
#include "game/Properties/2D/UI/UITypes.h"
#include "game/Property.h"
#include "game/Scene.h"
#include "game/Properties/2D/UI/UICanvas.h"

void UIInput::FromJSON(const json& j) {
	UIClickable::FromJSON(j);
	_initialText = j.value("text", "");
	_initialTextAlign = j.value("textAlign", AlignMap.enumToStr.at(Align::Start));

	if (j.contains("color")) {
		_initialColor = j["color"].get<std::array<float, 4>>();
	} else {
		_initialColor = { 0.6f, 0.6f, 0.6f, 1.0f };
	}

	if (j.contains("textColor")) {
		_initialTextColor = j["textColor"].get<std::array<float, 4>>();
	} else {
		_initialTextColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	_initialFont = j["font"];
	_initialFontSize = j["fontSize"];
}

void UIInput::OnCreate(std::weak_ptr<Scene> weakScene) {
	UIWidget::OnCreate(weakScene);

	auto scene = weakScene.lock();
	
	json widthChunk;
	if (_layout->GetWidth().mode == Size::Mode::Auto) {
		widthChunk = {
			{ "width", { { "mode", "Auto" } } }
		};
	} else {
		widthChunk = {
			{ "width", { { "mode", "Percent" }, { "value", 100 } } }
		};
	}

	json heightChunk;
	if (_layout->GetHeight().mode == Size::Mode::Auto) {
		heightChunk = {
			{ "height", { { "mode", "Auto" } } }
		};
	} else {
		heightChunk = {
			{ "height", { { "mode", "Percent" }, { "value", 100 } } }
		};
	}
	
	json layoutChunk = widthChunk;
	layoutChunk.merge_patch(heightChunk);

	json bgChunk = {
		{ "type", "GameObject" },
		{ "name", "bg" },
		{ "properties", {
			{ "UILayout", layoutChunk },
			{ "UIPanel", {
				{ "color", _initialColor },
				{ "flow", "Depth" },
			}}
		}}
	};
	_bg = scene->Instantiate(bgChunk, _gameObject.lock().get())->GetProperty<UIPanel>();

	json labelChunk = {
		{ "type", "GameObject" },
		{ "name", "text" },
		{ "properties", {
			{ "UILayout", layoutChunk },
			{ "UILabel", {
				{ "color", _initialTextColor },
				{ "font", _initialFont },
				{ "fontSize", _initialFontSize },
				{ "hAlign", _initialTextAlign },
				{ "text", _initialText }
			}}
		}}
	};
	_label = scene->Instantiate(labelChunk, _bg->GetGameObject().lock().get())->GetProperty<UILabel>();
	
	json cursorChunk = {
		{ "type", "GameObject" },
		{ "name", "cursor" },
		{ "active", false },
		{ "properties", {
			{ "UILayout", {
				{ "width", { { "mode", "Pixels" }, { "value", 2 } } },
				{ "height", { { "mode", "Percent" }, { "value", 90 } } }
			}},
			{ "UIIcon", {
				{ "color", { 0.0f, 0.0f, 0.0f, 1.0f }}
			}}
		}}
	};
	_cursor = scene->Instantiate(cursorChunk, _gameObject.lock().get())->GetProperty<UIIcon>();
	{
		glm::vec2 size = _label->MeasureText(_initialText);
		_cursor->GetLayout()->SetMargin({ size.y * (1.0f - 0.9f) / 2.0f, 0.0f, 0.0f, size.x });
		_cursor->MarkDirty();
	}

	json inputHandlerChunk = {
		{ "InputHandler", {
			{ "initialValue", _initialText }
		}}
	};
	_inputHandler = _gameObject.lock()->AddProperty<InputHandler>(inputHandlerChunk);
	
	_inputHandler->onCursorMoved.Subscribe([this](int cursor, std::string text) {
		std::string before = text.substr(0, cursor);
		glm::vec2 size = _label->MeasureText(before);
		_cursor->GetLayout()->SetMargin({ size.y * (1.0f - 0.90f) / 2.0f, 0.0f, 0.0f, size.x });
		_cursor->MarkDirty();
	});
	_inputHandler->onChange.Subscribe([this](std::string str) {
		onChange.Call(str);
	});
	_inputHandler->onCommit.Subscribe([this](std::string str) {
		onCommit.Call(str);
	});
	_inputHandler->onBlurRequested.Subscribe([this]() {
		if (auto canvas = _canvas.lock()) {
			canvas->Focus(nullptr);
		}
	});

	onChange.Subscribe([this](std::string str) {
		_label->SetText(str);
	});
	onClick.Subscribe([this]() {
		if (auto canvas = _canvas.lock()) {
			canvas->Focus(this);
		}
	});
}

void UIInput::OnFocus() {
	_inputHandler->SetFocused(true);
	_bg->SetColor(glm::vec4(1.0f));
	_cursor->GetGameObject().lock()->SetActive(true);
	_cursor->MarkDirty();
}

void UIInput::OnBlur() {
	_inputHandler->SetFocused(false);
	_bg->SetColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	_cursor->GetGameObject().lock()->SetActive(false);
}

REGISTER_PROPERTY(UIInput)
