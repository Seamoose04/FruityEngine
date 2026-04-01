#pragma once

#include <string>
#include <json.hpp>
#include <unordered_map>

using json = nlohmann::json;
class EditorScene {
public:
	void Load(const std::string& path);
	void Save();

	static EditorScene& Instance();

	const std::vector<std::string>& GetObjectPaths() const;
	const json& GetDocument(const std::string& document) const;
	json* GetNodeAtPath(const std::string& filePath, const std::string& nodePath);

private:
	json* _TraverseNode(json& node, std::vector<std::string>& segments, int index);

	std::string _scenePath;
	std::vector<std::string> _objectPaths;
	std::unordered_map<std::string, json> _documents;
};
