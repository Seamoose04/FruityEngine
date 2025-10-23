#pragma once
#include "core/FileLoader.h"
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

class JSONLoader : public FileLoader {
public:
    bool CanLoad(const std::string& path) const override;

    void* Load(const std::string& path) override;
};