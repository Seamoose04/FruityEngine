#include "PostProcessVolume.h"

#include <iostream>
#include <format>

#include "graphics/Renderer.h"

void PostProcessVolume::FromJSON(const json& data) {
    if (!data.contains("effects") || !data["effects"].is_array()) {
        std::cerr << "[PostProcessVolume] Missing 'effects' array in JSON" << std::endl;
        return;
    }
    _BuildGraph(data["effects"]);
    _WireInputs(data["effects"]);
}

void PostProcessVolume::OnCreate(std::weak_ptr<Scene> scene) {
    if (!_outputNode) {
        std::cerr << "[PostProcessVolume] No RenderToScreen node in graph" << std::endl;
        return;
    }

	_registered = false;
}

void PostProcessVolume::Render(Renderer& renderer) {
    if (!_registered && _sceneNode && _outputNode) {
        int width = renderer.GetWidth();
        int height = renderer.GetHeight();

        for (auto& [id, effect] : _effects) {
            effect->Init(width, height);
        }

        renderer.SetPostProcessGraph(_sceneNode, _outputNode);
        _registered = true;
    }
}

void PostProcessVolume::OnDestroy() {
    _registered = false;
}

void PostProcessVolume::_BuildGraph(const json& effectsArray) {
    for (const auto& entry : effectsArray) {
        std::string id = entry.value("id", "");
        std::string type = entry.value("type", "");

        if (id.empty() || type.empty()) {
            std::cerr << "[PostProcessVolume] Effect missing 'id' or 'type'" << std::endl;
            return;
        }

        std::shared_ptr<PostProcessEffect> effect;

        if (type == "Scene") {
            auto node = std::make_shared<SceneEffect>();
            _sceneNode = node.get();
            effect = node;
        } else if (type == "RenderToScreen") {
            auto node = std::make_shared<RenderToScreenEffect>();
            _outputNode = node.get();
            effect = node;
        } else {
            effect = Registry<PostProcessEffect>::Instance().Create(type);
            if (!effect) {
                std::cerr << "[PostProcessVolume] Unknown Effect type: " << type << std::endl;
                continue;
            }
        }

        effect->FromJSON(entry);
        _effects[id] = effect;
    }
}

void PostProcessVolume::_WireInputs(const json& effectsArray) {
    static const std::vector<std::string> INPUT_SLOTS = {
        "input", "inputA", "inputB", "inputC"
    };

    for (const auto& entry : effectsArray) {
        std::string id = entry.value("id", "");
        if (id.empty() || _effects.find(id) == _effects.end()) {
            continue;
        }

        auto& effect = _effects[id];

        for (const auto& slot : INPUT_SLOTS) {
            if (entry.contains(slot)) {
                std::string inputId = entry[slot];
                auto it = _effects.find(inputId);
                if (it != _effects.end()) {
                    effect->SetInput(slot, it->second.get());
                } else {
                    std::cerr << std::format("[PostProcessVolume] Unknown input id '{}' for effect '{}'",
                        inputId, id) << std::endl;
                }
            }
        }
    }
}

REGISTER_PROPERTY(PostProcessVolume)
