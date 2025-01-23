#include "Logger.hpp"
#include "UIDiv.hpp"
#include "UIButtonImage.hpp"
#include "UIButtonText.hpp"
#include "UITextEntry.hpp"

UIDiv::UIDiv()
{
}

UIDiv::UIDiv(
    const nlohmann::json &config,
    const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)
{
    _identifier         = json_get<std::string>(config, "id");
    _topLeftX = json_get<float>(config, "topLeftX");
    _topLeftY = json_get<float>(config, "topLeftY");
    if (!config.contains("childrens") || !config.at("childrens").is_array()) {
        throw std::runtime_error("`childrens` not in json or bad format (array expected)");
    }
    for (const auto &elem : config.at("childrens")) {
        if (!elem.is_object()) {
            throw std::invalid_argument("in childrens, bad format (object expected)");
        }
        if (!elem.contains("type") || !elem.contains("id") || !elem.at("type").is_string()
            || !elem.at("id").is_string()) {
            throw std::invalid_argument(
                "in childrens, type|id: not in json or bad format (expected string)");
        }
        const auto uiType = json_get<std::string>(elem, "type");
        const auto id     = json_get<std::string>(elem, "id");
        if (uiType == "div") {
            _childrens.push_back(std::make_shared<UIDiv>(elem, assets));
            _childrensIndexes[id] = _childrens.size() - 1;
        } else if (uiType == "buttonImage") {
            _childrens.push_back(std::make_shared<UIButtonImage>(elem, assets));
            _childrensIndexes[id] = _childrens.size() - 1;
        } else if (uiType == "buttonText") {
            _childrens.push_back(std::make_shared<UIButtonText>(elem));
            _childrensIndexes[id] = _childrens.size() - 1;
        } else if (uiType == "textEntry") {
            _childrens.push_back(std::make_shared<UITextEntry>(elem));
            _childrensIndexes[id] = _childrens.size() - 1;
        } else {
            throw std::invalid_argument("in childrens, type `" + uiType + "` is unknown");
        }
    }
}

bool UIDiv::modify(
    const std::string &identifier,
    const std::string &key,
    const nlohmann::json &value,
    const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)
{
    auto found = false;

    if (identifier == _identifier) {
        if (key == "topLeftX" && value.is_number_float()) {
            _topLeftX = value.template get<float>();
        } else if (key == "topLeftY" && value.is_number_float()) {
            _topLeftY = value.template get<float>();
        } else {
            Logger::error("UIDiv::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    if (_childrensIndexes.contains(identifier)) {
        return _childrens[_childrensIndexes[identifier]]->modify(identifier, key, value, assets);
    }
    for (const auto &elem : _childrens) {
        if (typeid(UIDiv).name() == typeid(elem).name()) {
            found = elem->modify(identifier, key, value, assets);
        }
        if (found == true) {
            return true;
        }
    }
    return false;
}

void UIDiv::update(raylib::Window &window, float parentX, float parentY)
{
    for (auto &children : _childrens) {
        children->update(window, parentX + _topLeftX, parentY + _topLeftY);
    }
}

void UIDiv::draw(raylib::Window &window, float parentX, float parentY)
{
    for (auto &children : _childrens) {
        children->draw(window, parentX + _topLeftX, parentY + _topLeftY);
    }
}

const std::string &UIDiv::getId() const
{
    return _identifier;
}

void UIDiv::addChildren(std::shared_ptr<IUI> children)
{
    _childrens.push_back(children);
    _childrensIndexes[children->getId()] = _childrens.size() - 1;
}
