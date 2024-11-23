#include "INetwork.hpp"
#include "UIConf.hpp"

UIConf::UIConf(const std::string &file):
    _json(nlohmann::json::parse(file))
{
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
