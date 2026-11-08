#pragma once

#include "game/Field.h"

#include <concepts>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <iostream>
#include <type_traits> // IWYU pragma: keep - used in REGISTER_FIELD_FACTORY

#include "game/GameObject.h"

class FieldFactoryRegistry {
public:
	using FieldFactory = std::function<std::unique_ptr<GameObject>(const std::shared_ptr<Field>&)>;

    static FieldFactoryRegistry& Instance() {
        static FieldFactoryRegistry inst;
        return inst;
    }

	template<typename F>
	requires std::derived_from<F, Field>
    bool Register(FieldFactory factory) {
        auto [it, inserted] = _factories.emplace(typeid(F),	factory);
        return inserted;
    }

	template<typename F>
	requires std::derived_from<F, Field>
	std::unique_ptr<GameObject> Create(const std::shared_ptr<F>& field) const {
		std::type_index key = typeid(F);
		auto it = _factories.find(key);
		if (it == _factories.end()) {
			std::cerr << "[FieldFactoryRegistry] Factory not found for Field: '" << key.name() << "'" << std::endl;
			std::abort();
		}
		return it->second(field);
	}

private:
    std::unordered_map<std::type_index, FieldFactory> _factories;
};

#define REGISTER_FIELD_FACTORY(FieldType, Factory)                        				 \
    namespace {                                                             			 \
		static_assert(std::is_invocable_r_v<std::unique_ptr<GameObject>, decltype(Factory), const std::shared_ptr<Field>&>, \
			"[FieldFactoryRegistry] '" #Factory "' does not match signature: std::unique_ptr<GameObject>(const std::shared_ptr<Field>&)"); \
        struct Factory##__AutoRegistrar {                               				 \
            Factory##__AutoRegistrar() {                                  				 \
                (void)FieldFactoryRegistry::Instance().Register<FieldType>(Factory); 	 \
            }                                                               			 \
        };                                                                  			 \
        static const Factory##__AutoRegistrar global_##Factory##_registrar; 		 	 \
    }
