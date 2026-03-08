#pragma once

#include <unordered_map>
#include <string>
#include <type_traits>
#include <initializer_list>

template <typename T>
class EnumMap {
    static_assert(std::is_enum_v<T>, "EnumMap<T>: T must be an enum type");
public:
    EnumMap(std::initializer_list<std::pair<std::string, T>> entries) {
        for (auto& [str, val] : entries) {
            strToEnum[str] = val;
            enumToStr[val] = str;
        }
    }
    
    std::unordered_map<std::string, T> strToEnum;
    std::unordered_map<T, std::string> enumToStr;
};