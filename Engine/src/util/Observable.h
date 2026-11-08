#pragma once

#include "util/Callback.h"

template<typename T>
class Observable {
public:
    Observable(const T& v = {}) : _value(v) {}

    const T& Get() const { return _value; }
    operator const T&() const { return _value; }
    const T* operator->() const { return &_value; }
    T* operator->() { return &_value; }
    const T& operator*() const { return _value; }
    T& operator*() { return _value; }

    void Set(const T& v) {
        if (_value != v) {
            _value = v;
            onChange.Call(_value);
        }
    }

	Callback<T> onChange;

private:
    T _value;
};

