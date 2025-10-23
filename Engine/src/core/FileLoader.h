#pragma once
#include <string>

class FileLoader {
public:
    virtual ~FileLoader() = default;
    virtual bool CanLoad(const std::string& path) const = 0;
    virtual void* Load(const std::string& path) = 0;
};