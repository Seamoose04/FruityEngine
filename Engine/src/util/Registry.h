#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

template<typename BaseType>
class Registry {
public:
    using Creator = std::function<std::shared_ptr<BaseType>()>;

    static Registry& Instance() {
        static Registry inst;
        return inst;
    }

    // returns false if a duplicate name was already registered
    bool Register(const std::string& name, Creator creator) {
        auto [it, inserted] = _creators.emplace(name, std::move(creator));
        return inserted;
    }

    std::shared_ptr<BaseType> Create(const std::string& name) const {
        auto it = _creators.find(name);
        if (it != _creators.end()) return (it->second)();
        return nullptr;
    }

private:
    std::unordered_map<std::string, Creator> _creators;
};

// Generic (by class name)
#define REGISTER_TYPE(BaseType, DerivedType)                                \
    namespace {                                                             \
        struct DerivedType##__AutoRegistrar {                               \
            DerivedType##__AutoRegistrar() {                                \
                (void)Registry<BaseType>::Instance().Register(              \
                    #DerivedType,                                           \
                    [] { return std::make_shared<DerivedType>(); }          \
                );                                                          \
            }                                                               \
        };                                                                  \
        static const DerivedType##__AutoRegistrar global_##DerivedType##_registrar; \
    }

// Convenience for scripts
#define REGISTER_SCRIPT(DerivedType) REGISTER_TYPE(Script, DerivedType)

// Optional: custom name if you ever need it
#define REGISTER_NAMED_TYPE(BaseType, DerivedType, NameLiteral)             \
    namespace {                                                             \
        struct DerivedType##__AutoRegistrarNamed {                          \
            DerivedType##__AutoRegistrarNamed() {                           \
                (void)Registry<BaseType>::Instance().Register(              \
                    NameLiteral,                                            \
                    [] { return std::make_shared<DerivedType>(); }          \
                );                                                          \
            }                                                               \
        };                                                                  \
        static const DerivedType##__AutoRegistrarNamed global_##DerivedType##_registrar_named; \
    }
