#pragma once

#include <unordered_map>
#include <functional>

template<typename... Args>
class Callback {
public:
	size_t Subscribe(std::function<void(Args...)> callback) {
		size_t id = _nextId++;
		_callbacks[id] = std::move(callback);
		return id;
	}
	void Unsubscribe(size_t id) {
		_callbacks.erase(id);
	}
	void Call(Args... args) {
		for (auto& [id, cb] : _callbacks) {
			cb(args...);
		}
	}

private:
	std::unordered_map<size_t, std::function<void(Args...)>> _callbacks;
	size_t _nextId = 0;
};
