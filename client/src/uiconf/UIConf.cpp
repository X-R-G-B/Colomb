#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include "INetwork.hpp"
#include "UIConf.hpp"

template<typename T>
std::optional<T> json_get(const nlohmann::json &json, const std::string &key);
template<> std::optional<int> json_get<int>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    if (!json.at(key).is_number()) {
        return std::nullopt;
    }
    return json.at(key).template get<int>();
}
template<> std::optional<float> json_get<float>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    if (!json.at(key).is_number_float()) {
        return std::nullopt;
    }
    return json.at(key).template get<float>();
}
template<> std::optional<bool> json_get<bool>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    if (!json.at(key).is_boolean()) {
        return std::nullopt;
    }
    return json.at(key).template get<bool>();
}
template<> std::optional<std::string> json_get<std::string>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    if (!json.at(key).is_string()) {
        return std::nullopt;
    }
    return json.at(key).template get<std::string>();
}
template<> std::optional<raylib::Color> json_get<raylib::Color>(const nlohmann::json &json, const std::string &key)
{
    if (!json.contains(key)) {
        return std::nullopt;
    }
    if (!json.at(key).is_array()) {
        return std::nullopt;
    }
    std::vector<int> vec;
    for (const auto &elem : json.at(key)) {
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

UIConf::UIConf(const std::string &file):
    _json(nlohmann::json::parse(file))
{
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
        if (!elem.contains("type") || !elem.contains("id") || !elem.at("type").is_string() || !elem.at("id").is_string()) {
            throw std::invalid_argument("in page, type|id: not in json or bad format (expected string)");
        }
        const auto uiType = elem.at("type").template get<std::string>();
        const auto id = elem.at("id").template get<std::string>();
        if (uiType == "div") {
            _page.push_back(std::make_shared<UIDiv>(elem, _assets));
            _pageIndexes[id] = _page.size() - 1;
        } else if (uiType == "buttonImage") {
            _page.push_back(std::make_shared<UIButtonImage>(elem, _assets));
            _pageIndexes[id] = _page.size() - 1;
        } else if (uiType == "buttonText") {
            _page.push_back(std::make_shared<UIButtonText>(elem));
            _pageIndexes[id] = _page.size() - 1;
        } else if (uiType == "textEntry") {
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
        if (!value.contains("type") || !value.contains("id") || !value.at("type").is_string() || !value.at("id").is_string()) {
            throw std::invalid_argument("in popups, type|id: not in json or bad format (expected string)");
        }
        const auto uiType = value.at("type").template get<std::string>();
        const auto id = value.at("id").template get<std::string>();
        if (uiType == "popups") {
            _popups[key] = std::make_shared<UIPopUp>(id, value);
        } else {
            throw std::invalid_argument("in popups, type `" + uiType + "` is unknown");
        }
    }
}

// ---------------------------------------------------------------------------
//                                                                       UIDiv
// ---------------------------------------------------------------------------

UIConf::UIDiv::UIDiv()
{
}

UIConf::UIDiv::UIDiv(const nlohmann::json &config, const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)
{
    _identifier = json_get<std::string>(config, "id").value();
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX = topLeftX.value();
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
        if (!elem.contains("type") || !elem.contains("id") || !elem.at("type").is_string() || !elem.at("id").is_string()) {
            throw std::invalid_argument("in childrens, type|id: not in json or bad format (expected string)");
        }
        const auto uiType = elem.at("type").template get<std::string>();
        const auto id = elem.at("id").template get<std::string>();
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

UIConf::UIButtonImage::UIButtonImage(const nlohmann::json &config, const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)
{
    _identifier = json_get<std::string>(config, "id").value();
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX = topLeftX.value();
    const auto topLeftY = json_get<float>(config, "topLeftY");
    if (!topLeftY.has_value()) {
        throw std::runtime_error("`topLeftY` not in json or bad format (float expected)");
    }
    _topLeftY = topLeftY.value();
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
    _visible = visible.value();
    const auto clickable = json_get<bool>(config, "clickable");
    if (!clickable.has_value()) {
        throw std::runtime_error("`clickable` not in json or bad format (bool expected)");
    }
    _clickable = clickable.value();
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
    const auto mouse = raylib::Mouse::GetPosition();
    const auto textureSize = _texture.GetSize();
    const auto rect = raylib::Rectangle(parentX + _topLeftX, parentY + _topLeftY, textureSize.x, textureSize.y);
    if (!rect.CheckCollision(mouse)) {
        return;
    }
    network.send({
            {"type", "uiUpdate_button"},
            {"id", this->getId()},
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
    _identifier = json_get<std::string>(config, "id").value();
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX = topLeftX.value();
    const auto topLeftY = json_get<float>(config, "topLeftY");
    if (!topLeftY.has_value()) {
        throw std::runtime_error("`topLeftY` not in json or bad format (float expected)");
    }
    _topLeftY = topLeftY.value();
    const auto text = json_get<std::string>(config, "text");
    if (!text.has_value()) {
        throw std::runtime_error("`text` not in json or bad format (string expected)");
    }
    _text = text.value();
    const auto bgColor = json_get<raylib::Color>(config, "bgColor");
    if (!bgColor.has_value()) {
        throw std::runtime_error("`bgColor` not in json or bad format (array expected)");
    }
    _bgColor = bgColor.value();
    const auto fgColor = json_get<raylib::Color>(config, "fgColor");
    if (!fgColor.has_value()) {
        throw std::runtime_error("`fgColor` not in json or bad format (array expected)");
    }
    _fgColor = fgColor.value();
    const auto visible = json_get<bool>(config, "visible");
    if (!visible.has_value()) {
        throw std::runtime_error("`visible` not in json or bad format (bool expected)");
    }
    _visible = visible.value();
    const auto clickable = json_get<bool>(config, "clickable");
    if (!clickable.has_value()) {
        throw std::runtime_error("`clickable` not in json or bad format (bool expected)");
    }
    _clickable = clickable.value();
}

void UIConf::UIButtonText::update(raylib::Window &window, float parentX, float parentY)
{
    if (_text != _textR.text) {
        _textR.text = _text;
    }
    if (!window.IsFocused()) {
        return;
    }
    if (!_clickable) {
        return;
    }
    if (!raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
        return;
    }
    const auto mouse = raylib::Mouse::GetPosition();
    const auto textSize = _textR.MeasureEx();
    const auto rect = raylib::Rectangle(parentX + _topLeftX, parentY + _topLeftY, textSize.x, textSize.y);
    if (!rect.CheckCollision(mouse)) {
        return;
    }
    network.send({
            {"type", "uiUpdate_button"},
            {"id", this->getId()},
        });
}

void UIConf::UIButtonText::draw(raylib::Window & /* unused */, float parentX, float parentY)
{
    if (!_visible) {
        return;
    }
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
    _identifier = json_get<std::string>(config, "id").value();
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX = topLeftX.value();
    const auto topLeftY = json_get<float>(config, "topLeftY");
    if (!topLeftY.has_value()) {
        throw std::runtime_error("`topLeftY` not in json or bad format (float expected)");
    }
    _topLeftY = topLeftY.value();
    const auto placeholder = json_get<std::string>(config, "placeholder");
    if (!placeholder.has_value()) {
        throw std::runtime_error("`placeholder` not in json or bad format (string expected)");
    }
    _placeholder = placeholder.value();
    _textR.text = _placeholder;
    const auto bgColor = json_get<raylib::Color>(config, "bgColor");
    if (!bgColor.has_value()) {
        throw std::runtime_error("`bgColor` not in json or bad format (array expected)");
    }
    _bgColor = bgColor.value();
    const auto fgColor = json_get<raylib::Color>(config, "fgColor");
    if (!fgColor.has_value()) {
        throw std::runtime_error("`fgColor` not in json or bad format (array expected)");
    }
    _fgColor = fgColor.value();
    const auto visible = json_get<bool>(config, "visible");
    if (!visible.has_value()) {
        throw std::runtime_error("`visible` not in json or bad format (bool expected)");
    }
    _visible = visible.value();
    const auto clickable = json_get<bool>(config, "clickable");
    if (!clickable.has_value()) {
        throw std::runtime_error("`clickable` not in json or bad format (bool expected)");
    }
    _clickable = clickable.value();
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
                    {"type", "uiUpdate_textEntry"},
                    {"id", this->getId()},
                    {"entry", _textR.text},
                });
        }
    }
    if (!raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
        return;
    }
    const auto mouse = raylib::Mouse::GetPosition();
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
    _identifier = id;
    const auto topLeftX = json_get<float>(config, "topLeftX");
    if (!topLeftX.has_value()) {
        throw std::runtime_error("`topLeftX` not in json or bad format (float expected)");
    }
    _topLeftX = topLeftX.value();
    const auto topLeftY = json_get<float>(config, "topLeftY");
    if (!topLeftY.has_value()) {
        throw std::runtime_error("`topLeftY` not in json or bad format (float expected)");
    }
    _topLeftY = topLeftY.value();
    const auto text = json_get<std::string>(config, "text");
    if (!text.has_value()) {
        throw std::runtime_error("`text` not in json or bad format (string expected)");
    }
    _text = text.value();
    const auto bgColor = json_get<raylib::Color>(config, "bgColor");
    if (!bgColor.has_value()) {
        throw std::runtime_error("`bgColor` not in json or bad format (array expected)");
    }
    _bgColor = bgColor.value();
    const auto fgColor = json_get<raylib::Color>(config, "fgColor");
    if (!fgColor.has_value()) {
        throw std::runtime_error("`fgColor` not in json or bad format (array expected)");
    }
    _fgColor = fgColor.value();
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

void UIConf::UIPopUp::update(raylib::Window &window, float parentX, float parentY)
{
    if (_text != _textR.text) {
        _textR.text = _text;
    }
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
        const auto rect = raylib::Rectangle(parentX + _topLeftX, parentY + _topLeftY, textSize.x, textSize.y);
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
                {"type", "uiUpdate_popup"},
                {"id", this->getId()},
                {"choice", key},
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

// ---------------------------------------------------------------------------
//                                                                       Asset
// ---------------------------------------------------------------------------

UIConf::Asset::Asset(const std::string &identifier, const std::string &urlPath): _identifier(identifier), _urlPath(urlPath)
{
}
