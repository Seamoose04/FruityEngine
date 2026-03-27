#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <iostream>

class ActionRegistry {
public:
    using Action = std::function<void()>;

    static ActionRegistry& Instance() {
        static ActionRegistry inst;
        return inst;
    }

    bool Register(const std::string& ns, const std::string& name, Action action) {
        std::string key = ns + "/" + name;
		std::cout << "Action '" << key << "' registered!" << std::endl;
        auto [it, inserted] = _actions.emplace(key, std::move(action));
        if (!inserted) {
            std::cerr << "[ActionRegistry] Duplicate action registered: \"" << key << "\"\n";
            std::abort();
        }
        return true;
    }

    void Execute(const std::string& key) const {
        auto it = _actions.find(key);
        if (it == _actions.end()) {
            std::cerr << "[ActionRegistry] Action not found: \"" << key << "\"\n";
            std::abort();
        }
        it->second();
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
