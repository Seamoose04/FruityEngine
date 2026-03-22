#pragma once

#include <unordered_map>
#include <functional>

class Callback {
public:
	size_t Subscribe(std::function<void()> callback);
	void Unsubscribe(size_t id);
	void Call();

private:
	std::unordered_map<size_t, std::function<void()>> _callbacks;
	size_t _nextId = 0;
};
