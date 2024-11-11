#include "Button.hpp"
#include "Logger.hpp"

Button::Button(raylib::Vector2 position, std::string texture, std::string text)
    : _position(position),
      _texture(texture)
{
    _text.text = text;
}

bool Button::isClicked(raylib::Window &window) const
{
    if (!window.IsFocused()) {
        return false;
    }
    if (!raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
        return false;
    }
    const auto x      = raylib::Mouse::GetX();
    const auto y      = raylib::Mouse::GetY();
    const auto size_y = _texture.GetHeight();
    const auto size_x = _texture.GetWidth();
    if (y < _position.GetY() || x < _position.GetX() || y > _position.GetY() + size_y
        || x > _position.GetX() + size_x) {
        return false;
    }
    return true;
}

void Button::draw(raylib::Window & /*unused*/) const
{
    _texture.Draw(_position);
    const auto size_text = _text.MeasureEx();
    auto pos_text = _position;
    pos_text.SetX(pos_text.GetX() + (size_text.GetX() / 2));
    pos_text.SetY(pos_text.GetY() + (size_text.GetY() / 2));
    _text.Draw(pos_text);
}

Button &Button::operator=(const Button &src)
{
    this->_text.text = src._text.text;
    this->_position = src._position;
    this->_texture = raylib::Texture(src._texture.GetData());
    return *this;
}
