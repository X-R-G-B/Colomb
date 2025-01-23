#include "INetwork.hpp"
#include "Logger.hpp"
#include "UIButtonText.hpp"

UIButtonText::UIButtonText()
{
}

UIButtonText::UIButtonText(const nlohmann::json &config)
{
    _identifier         = json_get<std::string>(config, "id");
    _topLeftX = json_get<float>(config, "topLeftX");
    _topLeftY = json_get<float>(config, "topLeftY");
    _text = json_get<std::string>(config, "text");
    _bgColor = json_get<raylib::Color>(config, "bgColor");
    _fgColor = json_get<raylib::Color>(config, "fgColor");
    _fgSize = json_get<float>(config, "fgSize");
    _visible = json_get<bool>(config, "visible");
    _clickable = json_get<bool>(config, "clickable");
}

bool UIButtonText::modify(
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
            Logger::error("UIButtonText::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    return false;
}

void UIButtonText::update(raylib::Window &window, float parentX, float parentY)
{
    if (_text != _textR.text)
        _textR.text = _text;
    if (_fgColor != raylib::Color(_textR.GetColor()))
        _textR.SetColor(_fgColor);
    if (_textR.GetFontSize() != _fgSize)
        _textR.SetFontSize(_fgSize);
    if (_textR.GetSpacing() != _fgSpacing)
        _textR.SetSpacing(_fgSpacing);
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

void UIButtonText::draw(raylib::Window & /* unused */, float parentX, float parentY)
{
    if (!_visible) {
        return;
    }
    _rectR.Draw(_bgColor);
    _textR.Draw(parentX + _topLeftX, parentY + _topLeftY);
}

const std::string &UIButtonText::getId() const
{
    return _identifier;
}
