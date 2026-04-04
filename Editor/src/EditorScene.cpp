#include "EditorScene.h"
#include "core/FileLoaders/JSONLoader.h"
#include <iostream>
#include <sstream>

EditorScene& EditorScene::Instance() {
	static EditorScene instance;
	return instance;
}

void EditorScene::Load(const std::string& path) {
	_scenePath = path;
	auto pos = _scenePath.find_last_of("/");
	std::string objectsPath = _scenePath.substr(0, pos + 1);

	JSONLoader loader;
	const json& j = loader.LoadJSON(_scenePath);
	_objectPaths = j["objects"].get<std::vector<std::string>>();
	for (auto& objPath : _objectPaths) {
		_documents[objPath] = loader.LoadJSON(objectsPath + objPath);
	}
}

json* EditorScene::_TraverseNode(json& node, std::vector<std::string>& segments, int index) {
	if (index >= segments.size()) {
		return &node;
	}
	if (!node.contains("children")) {
		std::cerr << "[EditorScene] Children not found." << std::endl;
	}
	json& children = node["children"];
	for (auto& child : children) {
		if (child["name"] == segments[index]) {
			return _TraverseNode(child, segments, index + 1);
		}
	}
	std::cerr << "[EditorScene] Child not found." << std::endl;
	return nullptr;
}

json* EditorScene::GetNodeAtPath(const std::string& filePath, const std::string& nodePath) {
	if (nodePath.empty()) {
		return &_documents[filePath];
	}

	std::vector<std::string> segments;
	std::stringstream ss(nodePath);
	std::string segment;
	while (std::getline(ss, segment, '/')) {
		segments.push_back(segment);
		std::cout << segment << std::endl;
	}

	return _TraverseNode(_documents[filePath], segments, 1);
}

const json& EditorScene::GetDocument(const std::string& document) const {
	return _documents.at(document);
}

const std::vector<std::string>& EditorScene::GetObjectPaths() const {
	return _objectPaths;
}
