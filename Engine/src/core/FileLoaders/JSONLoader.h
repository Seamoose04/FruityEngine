#pragma once

#include "core/FileLoader.h"
#include <json.hpp>

using json = nlohmann::json;

class JSONLoader : public FileLoader {
public:
    bool CanLoad(const std::string& path) const override;

    void* Load(const std::string& path) override;
	json LoadJSON(const std::string& path);

private:
	json _ResolveImports(const json& j, const std::filesystem::path& currentDir, const json& overrides = {});
};
