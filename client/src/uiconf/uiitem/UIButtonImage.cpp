#include "Logger.hpp"
#include "INetwork.hpp"
#include "UIButtonImage.hpp"

UIButtonImage::UIButtonImage()
{
}

UIButtonImage::UIButtonImage(
    const nlohmann::json &config,
    const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)
{
    _identifier         = json_get<std::string>(config, "id");
    _topLeftX = json_get<float>(config, "topLeftX");
    _topLeftY = json_get<float>(config, "topLeftY");
    _image = assets.at(json_get<std::string>(config, "image"));
    _texture.Load(_image->_path);
    _visible             = json_get<bool>(config, "visible");
    _clickable = json_get<bool>(config, "clickable");
}

bool UIButtonImage::modify(
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
                "UIButtonImage::modify : {" + identifier + "}: unknown key {" + key + "}");
            return false;
        }
        return true;
    }
    return false;
}

void UIButtonImage::update(raylib::Window &window, float parentX, float parentY)
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

void UIButtonImage::draw(raylib::Window & /* unused */, float parentX, float parentY)
{
    if (!_visible) {
        return;
    }
    _texture.Draw(parentX + _topLeftX, parentY + _topLeftY);
}

const std::string &UIButtonImage::getId() const
{
    return _identifier;
}
