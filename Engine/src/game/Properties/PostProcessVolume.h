#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "game/Property.h"
#include "game/Scene.h"
#include "graphics/PostProcessEffect.h"
#include "graphics/effects/Scene.h"
#include "graphics/effects/RenderToScreen.h"

class PostProcessVolume : public Property {
public:
    void FromJSON(const json& data, std::weak_ptr<Scene> scene) override;
    void OnCreate() override;
    void OnDestroy() override;
    void Render(Renderer& renderer) override; 

private:
    std::unordered_map<std::string, std::shared_ptr<PostProcessEffect>> _effects;
    SceneEffect* _sceneNode = nullptr;
    bool _registered = false;
    RenderToScreenEffect* _outputNode = nullptr;

    void _BuildGraph(const json& effectsArray);
    void _WireInputs(const json& effectsArray);
};