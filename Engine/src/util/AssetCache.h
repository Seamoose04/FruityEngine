#pragma once
#include <memory>
#include <unordered_map>

template<typename T>
class AssetCache {
public:
    static AssetCache& Instance() {
        static AssetCache inst;
        return inst;
    }

	std::shared_ptr<T> Get(const std::string& path) {
		auto it = _cache.find(path);
		if (it != _cache.end()) {
			return it->second;
		}
		auto asset = T::Load(path);
		_cache.emplace(path, asset);
		return asset;
	}

private:
    std::unordered_map<std::string, std::shared_ptr<T>> _cache;
};
