#pragma once
#include "Registry.h"
#include <unordered_map>

template<typename BaseType>
class SingletonRegistry : public Registry<BaseType> {
public:
    static SingletonRegistry& Instance() {
        static SingletonRegistry inst;
        return inst;
    }

    std::shared_ptr<BaseType> GetOrCreate(const std::string& name) {
        auto it = _instances.find(name);
        if (it != _instances.end()) return it->second;

        auto instance = Registry<BaseType>::Create(name);
        if (instance) _instances.emplace(name, instance);
        return instance;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<BaseType>> _instances;
};

#define REGISTER_SINGLETON_TYPE(BaseType, DerivedType)                                    \
    namespace {                                                                           \
        struct DerivedType##__SingletonAutoRegistrar {                                    \
            DerivedType##__SingletonAutoRegistrar() {                                     \
                (void)SingletonRegistry<BaseType>::Instance().Register(                   \
                    #DerivedType,                                                         \
                    [] { return std::make_shared<DerivedType>(); }                        \
                );                                                                        \
            }                                                                             \
        };                                                                                \
        static const DerivedType##__SingletonAutoRegistrar global_##DerivedType##_singleton_registrar; \
    }
