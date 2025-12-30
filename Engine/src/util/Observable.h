#pragma once
#include <functional>
#include <vector>
#include <algorithm>

template<typename T>
class Observable {
public:
    using Callback = std::function<void(const T&)>;

    Observable() = default;
    explicit Observable(const T& v) : _value(v) {}

    const T& Get() const { return _value; }
    operator const T&() const { return _value; }
    const T* operator->() const { return &_value; }
    T* operator->() { return &_value; }
    const T& operator*() const { return _value; }
    T& operator*() { return _value; }

    void Set(const T& newValue) {
        if (_value != newValue) {
            _value = newValue;
            for (auto& cb : _callbacks) {
				cb(_value);
			}
        }
    }

    void OnChange(Callback cb) { _callbacks.push_back(std::move(cb)); }

private:
    T _value{};
    std::vector<Callback> _callbacks;
};

