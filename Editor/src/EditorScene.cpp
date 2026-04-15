#include "EditorScene.h"
#include "core/FileLoaders/JSONLoader.h"
#include <iostream>
#include <sstream>

EditorScene& EditorScene::Instance() {
	static EditorScene instance;
	return instance;
}

void EditorScene::_Load() {
	JSONLoader loader;
	const json& j = loader.LoadJSON(_scenePath);
	_objectPaths = j["objects"].get<std::vector<std::string>>();
	for (auto& objPath : _objectPaths) {
		_documents[objPath] = loader.LoadJSON((_workingDir / objPath).string());
	}
}

void EditorScene::Load(const std::string& path) {
	namespace fs = std::filesystem;

	_scenePath = fs::absolute(path);
	_sceneDir = _scenePath.parent_path();
	_workingDir = fs::path(".working");

	fs::create_directories(_workingDir);
	fs::copy(_sceneDir, _workingDir, fs::copy_options::overwrite_existing | fs::copy_options::recursive);

	_Load();
}

void EditorScene::Save() {
	
}

void EditorScene::Revert() {
	std::filesystem::copy(_sceneDir, _workingDir, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);

	_Load();
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

