#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader() = default;
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    bool Load(const std::string& vertexPath, const std::string& fragmentPath);

    void Use() const;
    unsigned int GetID() const { return id; }

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int id = 0;

    std::string LoadFile(const std::string& path);
    unsigned int CompileShader(unsigned int type, const std::string& source);
};
