#pragma once
#include "GameObject.h"
#include "Property.h"
#include <iostream>
#include <typeinfo>

template <typename T> class PropertyRef {
public:
	PropertyRef() = default;

	PropertyRef &From(std::weak_ptr<GameObject> parent) {
		_parent = parent.lock().get();
		return *this;
	}

	T *Get() {
		if (!_cached && _parent) {
			auto prop = _parent->GetProperty<T>();
			if (!prop) {
				std::cerr << "PropertyRef<" << typeid(T).name()
					<< ">::Get(): property not found\n";
				return nullptr;
			}
			_cached = prop;
		}
		return _cached;
	}

	const T *Get() const {
		if (!_cached && _parent) {
			auto prop = _parent->GetProperty<T>();
			if (!prop) {
				std::cerr << "PropertyRef<" << typeid(T).name()
					<< ">::Get() const: property not found\n";
				return nullptr;
			}
			_cached = prop.get();
		}
		return _cached;
	}

	T *operator->() { return Get(); }
	const T *operator->() const { return Get(); }

	T &operator*() { return *Get(); }
	const T &operator*() const { return *Get(); }

	explicit operator bool() const { return Get() != nullptr; }

private:
	GameObject *_parent = nullptr; // non-owning pointer
	mutable T *_cached = nullptr;  // cached resolved property
};
