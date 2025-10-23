#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>
#include <iostream>

template <typename Base>
class Registry {
public:
    using Creator = std::function<std::unique_ptr<Base>()>;

    static Registry<Base>& Instance() {
        static Registry<Base> instance;
        return instance;
    }

    void Register(const std::string& name, Creator creator) {
        if (_creators.find(name) != _creators.end()) {
            std::cerr << "Warning: '" << name << "' already registered.\n";
            return;
        }
        _creators[name] = std::move(creator);
        std::cout << "[Registry] Registered type: " << name << std::endl;
    }

    std::unique_ptr<Base> Create(const std::string& name) const {
        auto it = _creators.find(name);
        if (it != _creators.end())
            return it->second();

        std::cerr << "Error: unknown registry type '" << name << "'\n";
        return nullptr;
    }

    bool Contains(const std::string& name) const {
        return _creators.find(name) != _creators.end();
    }

    std::vector<std::string> List() const {
        std::vector<std::string> keys;
        keys.reserve(_creators.size());
        for (auto& [key, _] : _creators)
            keys.push_back(key);
        return keys;
    }

private:
    std::unordered_map<std::string, Creator> _creators;

    Registry() = default;
    Registry(const Registry&) = delete;
    Registry& operator=(const Registry&) = delete;
};

// =============================================================
// Generic registration helper
// -------------------------------------------------------------
// Example usage:
//   REGISTER_TYPE(Property, Transform)
//   REGISTER_TYPE(FileLoader, JsonLoader)
// =============================================================
#define REGISTER_TYPE(BaseType, DerivedType) \
    namespace { \
        struct DerivedType##__AutoRegistrar { \
            DerivedType##__AutoRegistrar() { \
                Registry<BaseType>::Instance().Register(DerivedType::TypeName(), [] { \
                    return std::make_unique<DerivedType>(); \
                }); \
            } \
        }; \
        static const DerivedType##__AutoRegistrar global_##DerivedType##_registrar; \
    }
