#pragma once
#include <memory>
#include <iostream>
#include <typeinfo>
#include "GameObject.h"
#include "Property.h"

template<typename T>
class PropertyRef {
public:
    PropertyRef() = default;
    PropertyRef(std::weak_ptr<GameObject> parent)
        : _parent(parent) {}

    void SetParent(std::weak_ptr<GameObject> parent) {
        _parent = parent;
    }

    // Call this in Start(), after all properties are added
    void Resolve() {
        _cached = nullptr;

        auto parentShared = _parent.lock();
        if (!parentShared) {
            std::cerr << "PropertyRef<" << typeid(T).name() 
                      << ">::Resolve(): parent expired\n";
            return;
        }

        auto prop = parentShared->GetProperty<T>();
        if (!prop) {
            std::cerr << "PropertyRef<" << typeid(T).name()
                      << ">::Resolve(): property not found in GameObject\n";
            return;
        }

        _cached = prop;
    }

	void ResolveFrom(Property* self) {
		_parent = self->GetParent();
		Resolve();
	}

    // Safe accessors
    T* Get() const {
        if (!_cached) {
            std::cerr << "PropertyRef<" << typeid(T).name()
                      << ">::Get(): not yet resolved\n";
            return nullptr;
        }
        return _cached;
    }

    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    explicit operator bool() const { return _cached != nullptr; }

private:
    std::weak_ptr<GameObject> _parent;
    T* _cached = nullptr;  // Cached raw pointer (no ownership)
};
