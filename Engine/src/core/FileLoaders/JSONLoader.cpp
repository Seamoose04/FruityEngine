#include "JSONLoader.h"

bool JSONLoader::CanLoad(const std::string& path) const {
    return path.ends_with(".json");
}

void* JSONLoader::Load(const std::string& path) {
    std::ifstream file(path);
    json j;
    file >> j;
    return new json(std::move(j));
}