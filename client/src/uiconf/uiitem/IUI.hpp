#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include "raylib-cpp.hpp"
#include "nlohmann/json.hpp"
#include "Asset.hpp"

template <typename T>
T json_get(const nlohmann::json &json);

template <>
inline raylib::Color json_get(const nlohmann::json &json)
{
    if (!json.is_array()) {
        throw std::runtime_error("JSON_GET: Color: Expected Array: Element is not an array");
    }
    std::vector<int> vec;
    for (const auto &elem : json) {
        if (vec.size() >= 4) {
            throw std::runtime_error("JSON_GET: Color: Expected 4 Number: Array is more than 4 number");
        }
        if (!elem.is_number()) {
            throw std::runtime_error("JSON_GET: Color: Expected Number: Value in array is not a number");
        }
        vec.push_back(elem.template get<int>());
    }
    if (vec.size() != 4) {
        throw std::runtime_error("JSON_GET: Color: Expected 4 Number: Array is not 4 number");
    }
    return raylib::Color(vec[0], vec[1], vec[2], vec[3]);
}

template <typename T>
T json_get(const nlohmann::json &json, const std::string &key);

template <>
inline int json_get<int>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        throw std::runtime_error("JSON_GET: Int: Key Does Not Exists: '" + key + "'");
    }
    if (!json.at(key).is_number()) {
        throw std::runtime_error("JSON_GET: Int: Expected Int: Value is not int");
    }
    return json.at(key).template get<int>();
}
template <>
inline float json_get<float>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        throw std::runtime_error("JSON_GET: Float: Key Does Not Exists: '" + key + "'");
    }
    if (!json.at(key).is_number_float()) {
        throw std::runtime_error("JSON_GET: Float: Expected Float: Value is not float");
    }
    return json.at(key).template get<float>();
}
template <>
inline bool json_get<bool>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        throw std::runtime_error("JSON_GET: Bool: Key Does Not Exists: '" + key + "'");
    }
    if (!json.at(key).is_boolean()) {
        throw std::runtime_error("JSON_GET: Bool: Expected Bool: Value is not bool");
    }
    return json.at(key).template get<bool>();
}
template <>
inline std::string json_get<std::string>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        throw std::runtime_error("JSON_GET: String: Key Does Not Exists: '" + key + "'");
    }
    if (!json.at(key).is_string()) {
        throw std::runtime_error("JSON_GET: String: Expected String: Value is not string");
    }
    return json.at(key).template get<std::string>();
}
template <>
inline raylib::Color json_get<raylib::Color>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        throw std::runtime_error("JSON_GET: Color: Key Does Not Exists: '" + key + "'");
    }
    return json_get<raylib::Color>(json.at(key));
}

class IUI {
    public:
        virtual ~IUI() = default;
        virtual bool modify(
            const std::string &identifier,
            const std::string &key,
            const nlohmann::json &value,
            const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)        = 0;
        virtual void update(raylib::Window &window, float parentX = 0, float parentY = 0) = 0;
        virtual void draw(raylib::Window &window, float parentX = 0, float parentY = 0)   = 0;
        virtual const std::string &getId() const                                          = 0;
};
