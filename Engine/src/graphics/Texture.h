#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>

class Texture {
public:
	Texture() = default;
	Texture(const std::string& path);
    Texture(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& other) noexcept;
    ~Texture();
	
	bool Load(const std::string& path);
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

private:
	unsigned int _id = 0;
};
