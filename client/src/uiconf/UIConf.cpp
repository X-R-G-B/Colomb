#include "UIConf.hpp"
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include "INetwork.hpp"
#include "Logger.hpp"
#include "PathResolver.hpp"
#include "Rectangle.hpp"
#include "httplib.h"

template <typename T>
std::optional<T> json_get(const nlohmann::json &json);
template <>
std::optional<raylib::Color> json_get(const nlohmann::json &json)
{
    if (!json.is_array()) {
        return std::nullopt;
    }
    std::vector<int> vec;
    for (const auto &elem : json) {
        if (vec.size() >= 4) {
            return std::nullopt;
        }
        if (!elem.is_number()) {
            return std::nullopt;
        }
        vec.push_back(elem.template get<int>());
    }
    if (vec.size() != 4) {
        return std::nullopt;
    }
    return raylib::Color(vec[0], vec[1], vec[2], vec[3]);
}

template <typename T>
std::optional<T> json_get(const nlohmann::json &json, const std::string &key);
template <>
std::optional<int> json_get<int>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    if (!json.at(key).is_number()) {
        return std::nullopt;
    }
    return json.at(key).template get<int>();
}
template <>
std::optional<float> json_get<float>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    if (!json.at(key).is_number_float()) {
        return std::nullopt;
    }
    return json.at(key).template get<float>();
}
template <>
std::optional<bool> json_get<bool>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    if (!json.at(key).is_boolean()) {
        return std::nullopt;
    }
    return json.at(key).template get<bool>();
}
template <>
std::optional<std::string> json_get<std::string>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    if (!json.at(key).is_string()) {
        return std::nullopt;
    }
    return json.at(key).template get<std::string>();
}
template <>
std::optional<raylib::Color> json_get<raylib::Color>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    return json_get<raylib::Color>(json.at(key));
}

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
        if (typeid(UIConf::UIDiv).name() == typeid(elem).name()) {
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

// ---------------------------------------------------------------------------
//                                                                       UIDiv
// ---------------------------------------------------------------------------

UIConf::UIDiv::UIDiv()
{
}

UIConf::UIDiv::UIDiv(
    const nlohmann::json &config,
    const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)
{
    _identifier         = json_get<std::string>(config, "id").value();
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX           = topLeftX.value();
    const auto topLeftY = json_get<float>(config, "topLeftY");
    if (!topLeftY.has_value()) {
        throw std::runtime_error("`topLeftY` not in json or bad format (float expected)");
    }
    _topLeftY = topLeftY.value();
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
        const auto uiType = elem.at("type").template get<std::string>();
        const auto id     = elem.at("id").template get<std::string>();
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

bool UIConf::UIDiv::modify(
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
            Logger::error("UIConf::UIDiv::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    if (_childrensIndexes.contains(identifier)) {
        return _childrens[_childrensIndexes[identifier]]->modify(identifier, key, value, assets);
    }
    for (const auto &elem : _childrens) {
        if (typeid(UIConf::UIDiv).name() == typeid(elem).name()) {
            found = elem->modify(identifier, key, value, assets);
        }
        if (found == true) {
            return true;
        }
    }
    return false;
}

void UIConf::UIDiv::update(raylib::Window &window, float parentX, float parentY)
{
    for (auto &children : _childrens) {
        children->update(window, parentX + _topLeftX, parentY + _topLeftY);
    }
}

void UIConf::UIDiv::draw(raylib::Window &window, float parentX, float parentY)
{
    for (auto &children : _childrens) {
        children->draw(window, parentX + _topLeftX, parentY + _topLeftY);
    }
}

const std::string &UIConf::UIDiv::getId() const
{
    return _identifier;
}

void UIConf::UIDiv::addChildren(std::shared_ptr<IUI> children)
{
    _childrens.push_back(children);
    _childrensIndexes[children->getId()] = _childrens.size() - 1;
}

// ---------------------------------------------------------------------------
//                                                               UIButtonImage
// ---------------------------------------------------------------------------

UIConf::UIButtonImage::UIButtonImage()
{
}

UIConf::UIButtonImage::UIButtonImage(
    const nlohmann::json &config,
    const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)
{
    _identifier         = json_get<std::string>(config, "id").value();
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX           = topLeftX.value();
    const auto topLeftY = json_get<float>(config, "topLeftY");
    if (!topLeftY.has_value()) {
        throw std::runtime_error("`topLeftY` not in json or bad format (float expected)");
    }
    _topLeftY          = topLeftY.value();
    const auto imageId = json_get<std::string>(config, "image");
    if (!imageId.has_value()) {
        throw std::runtime_error("`image` not in json or bad format (string expected)");
    }
    _image = assets.at(imageId.value());
    _texture.Load(_image->_path);
    const auto visible = json_get<bool>(config, "visible");
    if (!visible.has_value()) {
        throw std::runtime_error("`visible` not in json or bad format (bool expected)");
    }
    _visible             = visible.value();
    const auto clickable = json_get<bool>(config, "clickable");
    if (!clickable.has_value()) {
        throw std::runtime_error("`clickable` not in json or bad format (bool expected)");
    }
    _clickable = clickable.value();
}

bool UIConf::UIButtonImage::modify(
    const std::string &identifier,
    const std::string &key,
    const nlohmann::json &value,
    const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)
{
    if (identifier == _identifier) {
        if (key == "topLeftX" && value.is_number_float()) {
            _topLeftX = value.template get<float>();
        } else if (key == "topLeftY" && value.is_number_float()) {
            _topLeftY = value.template get<float>();
        } else if (key == "image" && value.is_string()) {
            const auto image = value.template get<std::string>();
            _image           = assets.at(image);
            _texture.Load(_image->_path);
        } else if (key == "visible" && value.is_boolean()) {
            _visible = value.template get<bool>();
        } else if (key == "clickable" && value.is_boolean()) {
            _clickable = value.template get<bool>();
        } else {
            Logger::error(
                "UIConf::UIButtonImage::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    return false;
}

void UIConf::UIButtonImage::update(raylib::Window &window, float parentX, float parentY)
{
    if (!window.IsFocused()) {
        return;
    }
    if (!_clickable) {
        return;
    }
    if (!raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
        return;
    }
    const auto mouse       = raylib::Mouse::GetPosition();
    const auto textureSize = _texture.GetSize();
    const auto rect =
        raylib::Rectangle(parentX + _topLeftX, parentY + _topLeftY, textureSize.x, textureSize.y);
    if (!rect.CheckCollision(mouse)) {
        return;
    }
    network.send({
        {"type", "uiUpdate_button"},
        {"id",   this->getId()    },
    });
}

void UIConf::UIButtonImage::draw(raylib::Window & /* unused */, float parentX, float parentY)
{
    if (!_visible) {
        return;
    }
    _texture.Draw(parentX + _topLeftX, parentY + _topLeftY);
}

const std::string &UIConf::UIButtonImage::getId() const
{
    return _identifier;
}

// ---------------------------------------------------------------------------
//                                                                UIButtonText
// ---------------------------------------------------------------------------

UIConf::UIButtonText::UIButtonText()
{
}

UIConf::UIButtonText::UIButtonText(const nlohmann::json &config)
{
    _identifier         = json_get<std::string>(config, "id").value();
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX           = topLeftX.value();
    const auto topLeftY = json_get<float>(config, "topLeftY");
    if (!topLeftY.has_value()) {
        throw std::runtime_error("`topLeftY` not in json or bad format (float expected)");
    }
    _topLeftY       = topLeftY.value();
    const auto text = json_get<std::string>(config, "text");
    if (!text.has_value()) {
        throw std::runtime_error("`text` not in json or bad format (string expected)");
    }
    _text              = text.value();
    const auto bgColor = json_get<raylib::Color>(config, "bgColor");
    if (!bgColor.has_value()) {
        throw std::runtime_error("`bgColor` not in json or bad format (array expected)");
    }
    _bgColor           = bgColor.value();
    const auto fgColor = json_get<raylib::Color>(config, "fgColor");
    if (!fgColor.has_value()) {
        throw std::runtime_error("`fgColor` not in json or bad format (array expected)");
    }
    _fgColor           = fgColor.value();
    const auto visible = json_get<bool>(config, "visible");
    if (!visible.has_value()) {
        throw std::runtime_error("`visible` not in json or bad format (bool expected)");
    }
    _visible             = visible.value();
    const auto clickable = json_get<bool>(config, "clickable");
    if (!clickable.has_value()) {
        throw std::runtime_error("`clickable` not in json or bad format (bool expected)");
    }
    _clickable = clickable.value();
}

bool UIConf::UIButtonText::modify(
    const std::string &identifier,
    const std::string &key,
    const nlohmann::json &value,
    const std::unordered_map<std::string, std::shared_ptr<Asset>> & /* unused */)
{
    if (identifier == _identifier) {
        if (key == "topLeftX" && value.is_number_float()) {
            _topLeftX = value.template get<float>();
        } else if (key == "topLeftY" && value.is_number_float()) {
            _topLeftY = value.template get<float>();
        } else if (key == "text" && value.is_string()) {
            _text = value.template get<std::string>();
        } else if (key == "bgColor") {
            const auto bgColor = json_get<raylib::Color>(value);
            if (!bgColor.has_value()) {
                return false;
            }
            _bgColor = bgColor.value();
        } else if (key == "fgColor") {
            const auto fgColor = json_get<raylib::Color>(value);
            if (!fgColor.has_value()) {
                return false;
            }
            _fgColor = fgColor.value();
        } else if (key == "visible" && value.is_boolean()) {
            _visible = value.template get<bool>();
        } else if (key == "clickable" && value.is_boolean()) {
            _clickable = value.template get<bool>();
        } else {
            Logger::error("UIConf::UIButtonText::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    return false;
}

void UIConf::UIButtonText::update(raylib::Window &window, float parentX, float parentY)
{
    if (_text != _textR.text)
        _textR.text = _text;
    if (_fgColor != raylib::Color(_textR.GetColor()))
        _textR.SetColor(_fgColor);
    const auto textSize = _textR.MeasureEx();
    if (_rectR.width != textSize.x)
        _rectR.SetWidth(textSize.x);
    if (_rectR.height != textSize.y)
        _rectR.SetHeight(textSize.y);
    _rectR.SetX(parentX + _topLeftX);
    _rectR.SetY(parentY + _topLeftY);
    if (!window.IsFocused()) {
        return;
    }
    if (!_clickable) {
        return;
    }
    if (!raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
        return;
    }
    const auto mouse    = raylib::Mouse::GetPosition();
    if (!_rectR.CheckCollision(mouse)) {
        return;
    }
    network.send({
        {"type", "uiUpdate_button"},
        {"id",   this->getId()    },
    });
}

void UIConf::UIButtonText::draw(raylib::Window & /* unused */, float parentX, float parentY)
{
    if (!_visible) {
        return;
    }
    _rectR.Draw(_bgColor);
    _textR.Draw(parentX + _topLeftX, parentY + _topLeftY);
}

const std::string &UIConf::UIButtonText::getId() const
{
    return _identifier;
}

// ---------------------------------------------------------------------------
//                                                                 UITextEntry
// ---------------------------------------------------------------------------

UIConf::UITextEntry::UITextEntry()
{
}

UIConf::UITextEntry::UITextEntry(const nlohmann::json &config)
{
    _identifier         = json_get<std::string>(config, "id").value();
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX           = topLeftX.value();
    const auto topLeftY = json_get<float>(config, "topLeftY");
    if (!topLeftY.has_value()) {
        throw std::runtime_error("`topLeftY` not in json or bad format (float expected)");
    }
    _topLeftY              = topLeftY.value();
    const auto placeholder = json_get<std::string>(config, "placeholder");
    if (!placeholder.has_value()) {
        throw std::runtime_error("`placeholder` not in json or bad format (string expected)");
    }
    _placeholder       = placeholder.value();
    _textR.text        = _placeholder;
    const auto bgColor = json_get<raylib::Color>(config, "bgColor");
    if (!bgColor.has_value()) {
        throw std::runtime_error("`bgColor` not in json or bad format (array expected)");
    }
    _bgColor           = bgColor.value();
    const auto fgColor = json_get<raylib::Color>(config, "fgColor");
    if (!fgColor.has_value()) {
        throw std::runtime_error("`fgColor` not in json or bad format (array expected)");
    }
    _fgColor           = fgColor.value();
    const auto visible = json_get<bool>(config, "visible");
    if (!visible.has_value()) {
        throw std::runtime_error("`visible` not in json or bad format (bool expected)");
    }
    _visible             = visible.value();
    const auto clickable = json_get<bool>(config, "clickable");
    if (!clickable.has_value()) {
        throw std::runtime_error("`clickable` not in json or bad format (bool expected)");
    }
    _clickable = clickable.value();
}

bool UIConf::UITextEntry::modify(
    const std::string &identifier,
    const std::string &key,
    const nlohmann::json &value,
    const std::unordered_map<std::string, std::shared_ptr<Asset>> & /* unused */)
{
    if (identifier == _identifier) {
        if (key == "topLeftX" && value.is_number_float()) {
            _topLeftX = value.template get<float>();
        } else if (key == "topLeftY" && value.is_number_float()) {
            _topLeftY = value.template get<float>();
        } else if (key == "placeholder" && value.is_string()) {
            _placeholder = value.template get<std::string>();
        } else if (key == "bgColor") {
            const auto bgColor = json_get<raylib::Color>(value);
            if (!bgColor.has_value()) {
                return false;
            }
            _bgColor = bgColor.value();
        } else if (key == "fgColor") {
            const auto fgColor = json_get<raylib::Color>(value);
            if (!fgColor.has_value()) {
                return false;
            }
            _fgColor = fgColor.value();
        } else if (key == "visible" && value.is_boolean()) {
            _visible = value.template get<bool>();
        } else if (key == "clickable" && value.is_boolean()) {
            _clickable = value.template get<bool>();
        } else {
            Logger::error("UIConf::UITextEntry::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    return false;
}

void UIConf::UITextEntry::update(raylib::Window &window, float parentX, float parentY)
{
    if (!window.IsFocused()) {
        return;
    }
    if (!_clickable || !_visible) {
        return;
    }
    if (_wasClicked) {
        auto c = GetCharPressed();
        while (c != 0) {
            if ((c >= 32) && (c <= 125)) {
                _textR.text.push_back(c);
            }
            c = GetCharPressed();
        }
        if (raylib::Keyboard::IsKeyPressed(KEY_BACKSPACE) && _textR.text.length() != 0) {
            _textR.text.erase(_textR.text.length() - 1);
        }
        if (raylib::Keyboard::IsKeyPressed(KEY_ENTER) && _textR.text.length() != 0) {
            _wasClicked = false;
            network.send({
                {"type",  "uiUpdate_textEntry"},
                {"id",    this->getId()       },
                {"entry", _textR.text         },
            });
        }
    }
    if (!raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
        return;
    }
    const auto mouse    = raylib::Mouse::GetPosition();
    const auto textSize = _textR.MeasureEx();
    if (_textR.text == "") {
        textSize.Add(raylib::Vector2(20, 20));
    }
    const auto rect = raylib::Rectangle(parentX + _topLeftX, parentY + _topLeftY, textSize.x, textSize.y);
    if (!rect.CheckCollision(mouse)) {
        _wasClicked = false;
        return;
    }
    _wasClicked = true;
}

void UIConf::UITextEntry::draw(raylib::Window & /* unused */, float parentX, float parentY)
{
    if (!_visible) {
        return;
    }
    _textR.Draw(parentX + _topLeftX, parentY + _topLeftY);
}

const std::string &UIConf::UITextEntry::getId() const
{
    return _identifier;
}

// ---------------------------------------------------------------------------
//                                                                     UIPopUp
// ---------------------------------------------------------------------------

UIConf::UIPopUp::UIPopUp()
{
}

UIConf::UIPopUp::UIPopUp(const std::string &id, const nlohmann::json &config)
{
    _identifier         = id;
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX           = topLeftX.value();
    const auto topLeftY = json_get<float>(config, "topLeftY");
    if (!topLeftY.has_value()) {
        throw std::runtime_error("`topLeftY` not in json or bad format (float expected)");
    }
    _topLeftY       = topLeftY.value();
    const auto text = json_get<std::string>(config, "text");
    if (!text.has_value()) {
        throw std::runtime_error("`text` not in json or bad format (string expected)");
    }
    _text              = text.value();
    const auto bgColor = json_get<raylib::Color>(config, "bgColor");
    if (!bgColor.has_value()) {
        throw std::runtime_error("`bgColor` not in json or bad format (array expected)");
    }
    _bgColor           = bgColor.value();
    const auto fgColor = json_get<raylib::Color>(config, "fgColor");
    if (!fgColor.has_value()) {
        throw std::runtime_error("`fgColor` not in json or bad format (array expected)");
    }
    _fgColor           = fgColor.value();
    const auto visible = json_get<bool>(config, "visible");
    if (!visible.has_value()) {
        throw std::runtime_error("`visible` not in json or bad format (bool expected)");
    }
    _visible = visible.value();
    if (!config.contains("choices") || !config.at("choices").is_object()) {
        throw std::runtime_error("`choices` not in json or bad format (object expected)");
    }
    for (const auto &[key, value] : config.at("choices").items()) {
        if (!value.is_string()) {
            throw std::runtime_error("in choices, bad format (string expected)");
        }
        _choices[key] = value.template get<std::string>();
    }
}

bool UIConf::UIPopUp::modify(
    const std::string &identifier,
    const std::string &key,
    const nlohmann::json &value,
    const std::unordered_map<std::string, std::shared_ptr<Asset>> & /* unused */)
{
    if (identifier == _identifier) {
        if (key == "topLeftX" && value.is_number_float()) {
            _topLeftX = value.template get<float>();
        } else if (key == "topLeftY" && value.is_number_float()) {
            _topLeftY = value.template get<float>();
        } else if (key == "text" && value.is_string()) {
            _text = value.template get<std::string>();
        } else if (key == "bgColor") {
            const auto bgColor = json_get<raylib::Color>(value);
            if (!bgColor.has_value()) {
                return false;
            }
            _bgColor = bgColor.value();
        } else if (key == "fgColor") {
            const auto fgColor = json_get<raylib::Color>(value);
            if (!fgColor.has_value()) {
                return false;
            }
            _fgColor = fgColor.value();
        } else if (key == "choices") {
            _choices.clear();
            for (const auto &[key, value] : value.items()) {
                if (!value.is_string()) {
                    return false;
                }
                _choices[key] = value.template get<std::string>();
            }
        } else if (key == "visible" && value.is_boolean()) {
            _visible = value.template get<bool>();
        } else {
            Logger::error("UIConf::UIPopUp::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    return false;
}

void UIConf::UIPopUp::update(raylib::Window &window, float parentX, float parentY)
{
    if (_text != _textR.text) {
        _textR.text = _text;
    }
    // TODO: create _choicesR
    if (!window.IsFocused()) {
        return;
    }
    if (!_visible) {
        return;
    }
    if (!raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
        return;
    }
    const auto mouse = raylib::Mouse::GetPosition();
    for (const auto &text : _choicesR) {
        const auto textSize = text.MeasureEx();
        const auto rect =
            raylib::Rectangle(parentX + _topLeftX, parentY + _topLeftY, textSize.x, textSize.y);
        if (!rect.CheckCollision(mouse)) {
            continue;
        }
        std::string key;
        for (const auto &[key_i, text_i] : _choices) {
            if (text_i == text.text) {
                key = key_i;
                break;
            }
        }
        network.send({
            {"type",   "uiUpdate_popup"},
            {"id",     this->getId()   },
            {"choice", key             },
        });
    }
}

void UIConf::UIPopUp::draw(raylib::Window & /* unused */, float parentX, float parentY)
{
    if (!_visible) {
        return;
    }
    _textR.Draw(parentX + _topLeftX, parentY + _topLeftY);
    float cursor_x = 0;
    float cursor_y = _textR.MeasureEx().y + 10;
    for (const auto &text : _choicesR) {
        text.Draw(parentX + _topLeftX + cursor_x, parentY + _topLeftY + cursor_y);
        cursor_x += text.MeasureEx().x + 10;
    }
}

const std::string &UIConf::UIPopUp::getId() const
{
    return _identifier;
}

bool UIConf::UIPopUp::getVisible() const
{
    return _visible;
}

// ---------------------------------------------------------------------------
//                                                                       Asset
// ---------------------------------------------------------------------------

UIConf::Asset::Asset(const std::string &identifier, const std::string &urlPath)
    : _identifier(identifier),
      _urlPath(urlPath)
{
    std::string host(_urlPath);
    host.erase(host.find('/', host.find("://") + 3));
    std::string path(_urlPath.substr(host.size()));

    Logger::info("UICONF_ASSET: Downloading host=" + host + "; path=" + path + ";");
    httplib::Client cli(host);

    cli.set_follow_location(true);
    const auto &res = cli.Get(path);
    if (res->status != 200) {
        throw std::runtime_error("This should not happend: failed to load assets");
    }
    const auto filePath = PathResolver::resolve(identifier);
    std::ofstream file(filePath);
    file << res->body;
    file.close();
    _path = filePath;
}
