#pragma once
#include "core/FileLoader.h"
#include "graphics/Mesh.h"

class OBJLoader : public FileLoader {
public:
    bool CanLoad(const std::string& path) const override;
    void* Load(const std::string& path) override;
};
