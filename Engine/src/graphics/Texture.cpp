#include "Texture.h"

#include <iostream>
#include <stb_image.h>

Texture::Texture(const std::string& path) {
	if (!Load(path)) {
		std::abort();
	}
}

bool Texture::Load(const std::string& path) {
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
	
	if (!data) {
		std::cerr << "[Texture] Failed to load: " << path << std::endl;
		return false;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	return true;
}

Texture::Texture(Texture&& other) noexcept {
    _id = other._id;
    other._id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (_id) glDeleteTextures(1, &_id);

        _id = other._id;
        other._id = 0;
    }
    return *this;
}

Texture::~Texture() {
	glDeleteTextures(1, &_id);
}

void Texture::Bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}
