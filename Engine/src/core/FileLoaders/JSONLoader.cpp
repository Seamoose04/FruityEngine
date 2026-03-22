#include "JSONLoader.h"

#include <fstream>
#include <iostream>

#include "graphics/PostProcessEffect.h"

bool JSONLoader::CanLoad(const std::string& path) const {
    return path.ends_with(".json");
}

void* JSONLoader::Load(const std::string& path) {
	json j = LoadJSON(path);
	j = _ResolveImports(j, std::filesystem::path(path).parent_path());
    return new json(std::move(j));
}

json JSONLoader::LoadJSON(const std::string& path) {
    std::ifstream file(path);
    json j;
    file >> j;
	return j;
}

json JSONLoader::_ResolveImports(const json& j, const std::filesystem::path& currentDir, const json& overrides) {
	json result;
	if (j.is_array()) {
		result = json::array();
		for (auto& element : j) {
			 result.push_back(_ResolveImports(element, currentDir, overrides));
		}
	} else if (j.is_object()) {
		result = json::object();
		std::string importPath = "";
		json importOverrides = json::object();
		for (auto& [key, value] : j.items()) {
			if (key == "$import") {
				importPath = value.get<std::string>();
			} else if (key == "$override") {
				importOverrides = value;
			} else {
				if (overrides.contains(key + "!")) {
					result[key] = _ResolveImports(overrides[key + "!"], currentDir, json::object());
				} else if (overrides.contains(key)) {
					if (overrides[key].is_primitive()) {
						result[key] = overrides[key];
					} else {
						result[key] = _ResolveImports(value, currentDir, overrides[key]);
					}
				} else {
					result[key] = _ResolveImports(value, currentDir, json::object());
				}
			}
		}
		if (result.empty()) {
			if (!importPath.empty()) {
				std::filesystem::path resolvedPath;
				if (importPath[0] == '.') {
					resolvedPath = std::filesystem::weakly_canonical(currentDir / importPath);
				} else {
					resolvedPath = std::filesystem::weakly_canonical(std::filesystem::path("assets/chunks") / importPath);
				}
				json imported = LoadJSON(resolvedPath.string());
				return _ResolveImports(imported, resolvedPath.parent_path(), importOverrides);
			} else if (!importOverrides.empty()) {
				std::cerr << "$override MUST have an accompanying import path." << std::endl;
			} else {
				std::cerr << "objects should not be empty." << std::endl;
			}
		}
		if (!importPath.empty() || !importOverrides.empty()) {
			std::cerr << "Properties and $import cannot exist at the same level." << std::endl;
		}
	} else {
		result = j;
	}

	return result;
}
