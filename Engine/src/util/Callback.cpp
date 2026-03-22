#include "Callback.h"

size_t Callback::Subscribe(std::function<void()> callback) {
	size_t id = _nextId++;
	_callbacks[id] = std::move(callback);
	return id;
}

void Callback::Unsubscribe(size_t id) {
	_callbacks.erase(id);
}

void Callback::Call() {
	for (auto& [id, cb] : _callbacks) {
		cb();
	}
}
