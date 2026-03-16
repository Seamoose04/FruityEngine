#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include "graphics/Vertex.h"

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
	std::vector<glm::vec3> tempPositions;
	std::vector<glm::vec3> tempNormals;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

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
			tempPositions.emplace_back(x, y, z);
		} else if (prefix == "vn") {
			float x, y, z;
			ss >> x >> y >> z;
		} else if (prefix == "f") {
			std::string vertexStr;
			while (ss >> vertexStr) {
				std::stringstream vs(vertexStr);
				std::string posStr, uvStr, normStr;
				std::getline(vs, posStr, '/');
				std::getline(vs, uvStr, '/');
				std::getline(vs, normStr, '/');

				unsigned int posIdx = std::stoi(posStr) - 1;
				unsigned int normIdx = normStr.empty() ? 0 : std::stoi(normStr) - 1;
				
				Vertex vert;
				vert.position = tempPositions[posIdx];
				vert.normal = normIdx < tempNormals.size() ? tempNormals[normIdx] : glm::vec3(0, 1, 0);

				indices.push_back(static_cast<unsigned int>(vertices.size()));
				vertices.push_back(vert);
			}
		}
	}

	Mesh m(vertices, indices);
	return new Mesh(std::move(m));
}
