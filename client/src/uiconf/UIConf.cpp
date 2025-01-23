#include "UIConf.hpp"
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include "Logger.hpp"
#include "PathResolver.hpp"
#include "UIDiv.hpp"
#include "UIButtonImage.hpp"
#include "UIButtonText.hpp"
#include "UITextEntry.hpp"


UIConf::UIConf(const std::string &file)
{
    std::ifstream f(file);
    _json = nlohmann::json::parse(f);
    if (!_json.contains("name") || !_json.at("name").is_string()) {
        throw std::invalid_argument("`name` not in json or bad format (string expected)");
    }
    _name = _json.at("name").template get<std::string>();
    if (!_json.contains("assets") || !_json.at("assets").is_object()) {
        throw std::invalid_argument("`assets` not in json or bad format (object expected)");
    }
    for (const auto &[key, value] : _json.at("assets").items()) {
        if (!value.is_string()) {
            throw std::invalid_argument("in asset, `" + key + "` bad format (string expected)");
        }
        _assets[key] = std::make_shared<Asset>(key, value.template get<std::string>());
    }
    if (!_json.contains("page") || !_json.at("page").is_array()) {
        throw std::invalid_argument("`page` not in json or bad format (array expected)");
    }
    for (const auto &elem : _json.at("page")) {
        if (!elem.is_object()) {
            throw std::invalid_argument("in page, bad format (object expected)");
        }
        if (!elem.contains("type") || !elem.contains("id") || !elem.at("type").is_string()
            || !elem.at("id").is_string()) {
            throw std::invalid_argument("in page, type|id: not in json or bad format (expected string)");
        }
        const auto uiType = elem.at("type").template get<std::string>();
        const auto id     = elem.at("id").template get<std::string>();
        if (uiType == "div") {
            _page.push_back(std::make_shared<UIDiv>(elem, _assets));
            _pageIndexes[id] = _page.size() - 1;
        } else if (uiType == "button_image") {
            _page.push_back(std::make_shared<UIButtonImage>(elem, _assets));
            _pageIndexes[id] = _page.size() - 1;
        } else if (uiType == "button_text") {
            _page.push_back(std::make_shared<UIButtonText>(elem));
            _pageIndexes[id] = _page.size() - 1;
        } else if (uiType == "text_entry") {
            _page.push_back(std::make_shared<UITextEntry>(elem));
            _pageIndexes[id] = _page.size() - 1;
        } else {
            throw std::invalid_argument("in page, type `" + uiType + "` is unknown");
        }
    }
    if (!_json.contains("popups") || !_json.at("popups").is_object()) {
        throw std::invalid_argument("`popups` not in json or bad format (object expected)");
    }
    for (const auto &[key, value] : _json.at("popups").items()) {
        if (!value.is_object()) {
            throw std::invalid_argument("in page, bad format (object expected)");
        }
        if (!value.contains("type") || !value.contains("id") || !value.at("type").is_string()
            || !value.at("id").is_string()) {
            throw std::invalid_argument("in popups, type|id: not in json or bad format (expected string)");
        }
        const auto uiType = value.at("type").template get<std::string>();
        const auto id     = value.at("id").template get<std::string>();
        if (uiType == "popups") {
            _popups[key] = std::make_shared<UIPopUp>(id, value);
        } else {
            throw std::invalid_argument("in popups, type `" + uiType + "` is unknown");
        }
    }
}

bool UIConf::modify(const std::string &identifier, const std::string &key, const nlohmann::json &value)
{
    auto found = false;

    if (_pageIndexes.contains(identifier)) {
        return _page[_pageIndexes[identifier]]->modify(identifier, key, value, _assets);
    }
    for (const auto &elem : _page) {
        if (typeid(UIDiv).name() == typeid(elem).name()) {
            found = elem->modify(identifier, key, value, _assets);
        }
        if (found == true) {
            return true;
        }
    }
    return false;
}

void UIConf::update(raylib::Window &window)
{
    bool inPopup = false;

    for (const auto &[_, elem] : _popups) {
        if (elem->getVisible()) {
            inPopup = true;
            elem->update(window);
        }
    }
    if (!inPopup) {
        for (const auto &elem : _page) {
            elem->update(window);
        }
    }
}

void UIConf::draw(raylib::Window &window)
{
    for (const auto &[_, elem] : _popups) {
        elem->draw(window);
    }
    for (const auto &elem : _page) {
        elem->draw(window);
    }
}

std::string UIConf::toFile(const std::string &identifier)
{
    const std::string file = PathResolver::resolve("assets/uiconf/" + identifier);
    std::filesystem::path path(file);

    if (std::filesystem::exists(path.parent_path())) {
        std::runtime_error("folder uiconf does not exist");
    }
    return file;
}

std::string UIConf::hash(const std::string &identifier)
{
    std::ifstream file(UIConf::toFile(identifier));
    std::string all;

    file.seekg(0, std::ios::end);
    all.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    all.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
    const std::string hashed = std::to_string(std::hash<std::string> {}(all));
    Logger::debug("hash_" + identifier + "_" + hashed);
    return hashed;
}

bool UIConf::appendString(const std::string &identifier, const std::string &data)
{
    std::ofstream file;

    file.open(UIConf::toFile(identifier), std::ios_base::app);
    if (file.is_open() && !file.bad()) {
        file << data;
        return true;
    }
    return false;
}

bool UIConf::writeString(const std::string &identifier, const std::string &data)
{
    std::ofstream file(UIConf::toFile(identifier));

    if (file.is_open() && !file.bad()) {
        file << data;
        return true;
    }
    return false;
}
