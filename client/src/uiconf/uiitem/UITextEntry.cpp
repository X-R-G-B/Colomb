#include "Logger.hpp"
#include "INetwork.hpp"
#include "UITextEntry.hpp"

UITextEntry::UITextEntry()
{
}

UITextEntry::UITextEntry(const nlohmann::json &config)
{
    _identifier         = json_get<std::string>(config, "id");
    _topLeftX = json_get<float>(config, "topLeftX");
    _topLeftY = json_get<float>(config, "topLeftY");
    _placeholder = json_get<std::string>(config, "placeholder");
    _textR.text        = _placeholder;
    _bgColor = json_get<raylib::Color>(config, "bgColor");
    _fgColor = json_get<raylib::Color>(config, "fgColor");
    _visible = json_get<bool>(config, "visible");
    _fgSize = json_get<float>(config, "fgSize");
    _clickable = json_get<bool>(config, "clickable");
}

bool UITextEntry::modify(
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
            _bgColor = json_get<raylib::Color>(value);
        } else if (key == "fgColor") {
            _fgColor = json_get<raylib::Color>(value);
        } else if (key == "fgSize") {
            _fgSize = value.template get<float>();
        } else if (key == "visible" && value.is_boolean()) {
            _visible = value.template get<bool>();
        } else if (key == "clickable" && value.is_boolean()) {
            _clickable = value.template get<bool>();
        } else {
            Logger::error("UITextEntry::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    return false;
}

void UITextEntry::update(raylib::Window &window, float parentX, float parentY)
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

void UITextEntry::draw(raylib::Window & /* unused */, float parentX, float parentY)
{
    if (!_visible) {
        return;
    }
    _textR.Draw(parentX + _topLeftX, parentY + _topLeftY);
}

const std::string &UITextEntry::getId() const
{
    return _identifier;
}
