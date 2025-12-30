#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool OBJLoader::CanLoad(const std::string &path) const {
  return path.ends_with(".obj");
}

void* OBJLoader::Load(const std::string& path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "[OBJLoader] Failed to open file: " << path << std::endl;
		Mesh m;
		return new Mesh(std::move(m));
	}

	std::string line;
	std::vector<glm::vec3> tempVertices;
	std::vector<unsigned int> tempIndices;

	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string prefix;
		ss >> prefix;

		if (prefix.empty() || prefix[0] == '#') {
			continue;
		}

		if (prefix == "v") {
			float x, y, z;
			ss >> x >> y >> z;
			tempVertices.emplace_back(x, y, z);
		} else if (prefix == "f") {
			std::string vertexStr;
			while (ss >> vertexStr) {
				std::stringstream vs(vertexStr);
				std::string indexStr;
				std::getline(vs, indexStr, '/');
				unsigned int vertexIndex = std::stoi(indexStr);
				tempIndices.push_back(vertexIndex - 1); // OBJ indices are 1-based
			}
		}
	}

	Mesh m(tempVertices, tempIndices);
	return new Mesh(std::move(m));
}
