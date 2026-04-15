#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "graphics/Shader.h"

class ShaderCache {
public:
    static ShaderCache& Instance() {
        static ShaderCache inst;
        return inst;
    }

    std::shared_ptr<Shader> Get(const std::string& vert, const std::string& frag) {
        std::string key = vert + "|" + frag;
        auto it = _cache.find(key);
        if (it != _cache.end())
            return it->second;
        auto shader = std::make_shared<Shader>(vert, frag);
        _cache.emplace(key, shader);
        return shader;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> _cache;
};
