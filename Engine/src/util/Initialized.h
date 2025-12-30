#pragma once
#include <iostream>

template<typename T>
class Initialized {
public:
	Initialized() = default;
	~Initialized() = default;

	// Assignment for first-time initialization
	Initialized<T>& operator=(const T& initial) {
		if (!_initialized) {
			_value = initial;
			_initialized = true;
		} else {
			std::cerr << "Tried to assign to an already initialized value" << std::endl;
		}
		return *this;
	}

	// Implicit conversion to T&
	operator T&() {
		if (!_initialized) {
			std::cerr << "Tried to access uninitialized value" << std::endl;
			throw std::runtime_error("Uninitialized value");
		}
		return _value;
	}

	// Implicit conversion to const T&
	operator const T&() const {
		if (!_initialized) {
			std::cerr << "Tried to access uninitialized value" << std::endl;
			throw std::runtime_error("Uninitialized value");
		}
		return _value;
	}

	// Pointer-like access (for structs/classes)
	T* operator->() {
		if (!_initialized) {
			std::cerr << "Tried to dereference uninitialized value" << std::endl;
			throw std::runtime_error("Uninitialized value");
		}
		return &_value;
	}

	const T* operator->() const {
		if (!_initialized) {
			std::cerr << "Tried to dereference uninitialized value" << std::endl;
			throw std::runtime_error("Uninitialized value");
		}
		return &_value;
	}

	// Manual getter
	T& Get() {
		if (!_initialized) {
			std::cerr << "Tried to get an uninitialized value" << std::endl;
			throw std::runtime_error("Uninitialized value");
		}
		return _value;
	}

	// Reset state
	void Reset() {
		_initialized = false;
	}

	bool IsInitialized() const { return _initialized; }

private:
	T _value{};
	bool _initialized = false;
};

