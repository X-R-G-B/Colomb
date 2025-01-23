#include "Logger.hpp"
#include "INetwork.hpp"
#include "UIPopUp.hpp"

UIPopUp::UIPopUp()
{
}

UIPopUp::UIPopUp(const std::string &id, const nlohmann::json &config)
{
    _identifier         = id;
    _topLeftX = json_get<float>(config, "topLeftX");
    _topLeftY = json_get<float>(config, "topLeftY");
    _text = json_get<std::string>(config, "text");
    _bgColor = json_get<raylib::Color>(config, "bgColor");
    _fgColor = json_get<raylib::Color>(config, "fgColor");
    _visible = json_get<bool>(config, "visible");
    _fgSize = json_get<float>(config, "fgSize");
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

bool UIPopUp::modify(
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
            Logger::error("UIPopUp::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    return false;
}

void UIPopUp::update(raylib::Window &window, float parentX, float parentY)
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

void UIPopUp::draw(raylib::Window & /* unused */, float parentX, float parentY)
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

const std::string &UIPopUp::getId() const
{
    return _identifier;
}

bool UIPopUp::getVisible() const
{
    return _visible;
}
