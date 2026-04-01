#pragma once
#include "json.hpp"
#include <functional>
#include <string>
#include <unordered_map>
#include <iostream>

using json = nlohmann::json;
class ActionRegistry {
public:
    using Action = std::function<void(const json&)>;

	struct ActionCall {
		std::string key = "";
		json args;

		bool empty() const { return key.empty(); }

		void FromJSON(const json& j) {
			key = j["key"].get<std::string>();
			args = j.value("args", json::object());
		}

		void Invoke() const {
			ActionRegistry::Instance().Invoke(key, args);
		}
	};

    static ActionRegistry& Instance() {
        static ActionRegistry inst;
        return inst;
    }

    bool Register(const std::string& ns, const std::string& name, Action action) {
        std::string key = ns + "/" + name;
        auto [it, inserted] = _actions.emplace(key, std::move(action));
        if (!inserted) {
            std::cerr << "[ActionRegistry] Duplicate action registered: \"" << key << "\"\n";
            std::abort();
        }
		std::cout << "Action '" << key << "' registered!" << std::endl;
        return true;
    }

	void Unregister(const std::string& ns, const std::string& name) {
		std::string key = ns + "/" + name;
		auto it = _actions.find(key);
		if (it == _actions.end()) {
			std::cerr << "[ActionRegistry] Tried to unregister '" << key << "' which does not exist." << std::endl;
		}
		std::cout << "Action '" << key << "' unregistered!" << std::endl;
		_actions.erase(it);
	}

	void Invoke(const std::string& key, json args) const {
		auto it = _actions.find(key);
		if (it == _actions.end()) {
			std::cerr << "[ActionRegistry] JAction not found: '" << key << "'" << std::endl;
			std::abort();
		}
		it->second(args);
	}

private:
    std::unordered_map<std::string, Action> _actions;
};

#define REGISTER_ACTION(Namespace, Name, Func)                                      \
    namespace {                                                                     \
        struct Action__AutoReg__##__COUNTER__ {                                     \
            Action__AutoReg__##__COUNTER__() {                                      \
                ActionRegistry::Instance().Register(Namespace, Name, Func);        \
            }                                                                       \
        };                                                                          \
        static const Action__AutoReg__##__COUNTER__ _reg;                          \
    }
