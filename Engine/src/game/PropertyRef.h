#pragma once
#include "GameObject.h"
#include "Property.h"
#include <iostream>
#include <typeinfo>

template <typename T> class PropertyRef {
public:
	PropertyRef() = default;

	PropertyRef &From(std::weak_ptr<GameObject> parent) {
		_parent = parent.lock();
		return *this;
	}

	T *Get() {
		if (auto cached = _cached.lock()) {
			return cached.get();
		}
		if (auto parent = _parent.lock()) {
			auto prop = parent->GetProperty<T>();
			if (!prop) {
				std::cerr << "PropertyRef<" << typeid(T).name()
					<< ">::Get(): property not found\n";
				return nullptr;
			}
			_cached = prop;
		}
		return _cached.lock().get();
	}

	const T *Get() const {
		if (auto cached = _cached.lock()) {
			return cached.get();
		}
		if (auto parent = _parent.lock()) {
			auto prop = parent->GetProperty<T>();
			if (!prop) {
				std::cerr << "PropertyRef<" << typeid(T).name()
					<< ">::Get() const: property not found\n";
				return nullptr;
			}
			_cached = prop;
		}
		return _cached.lock().get();
	}

	T *operator->() { return Get(); }
	const T *operator->() const { return Get(); }

	T &operator*() { return *Get(); }
	const T &operator*() const { return *Get(); }

	explicit operator bool() const { return Get() != nullptr; }

private:
	std::weak_ptr<GameObject> _parent; // non-owning pointer
	mutable std::weak_ptr<T> _cached;  // cached resolved property
};
