#include <memory>
#include <stdexcept>
#include "INetwork.hpp"
#include "UIConf.hpp"

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
        _assets[key] = Asset(key, value.template get<std::string>());
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
            _page.push_back(std::make_shared<UIDiv>(elem));
            _pageIndexes[id] = _page.size() - 1;
        } else if (uiType == "buttonImage") {
            _page.push_back(std::make_shared<UIButtonImage>(elem));
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
            _popups[key] = std::make_shared<UIPopUp>(value);
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

void UIConf::UIButtonText::update(raylib::Window &window, float parentX, float parentY)
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
    _textR.text = _placeholder;
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

void UIConf::UIPopUp::update(raylib::Window &window, float parentX, float parentY)
{
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
