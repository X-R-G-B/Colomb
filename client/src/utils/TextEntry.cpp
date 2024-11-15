#include "TextEntry.hpp"
#include "raylib.h"
#include "Keyboard.hpp"

TextEntry::TextEntry(
    raylib::Vector2 position,
    raylib::Vector2 size,
    raylib::Color bgColor,
    raylib::Color fgColor,
    int fgSize,
    float fgSpacing,
    int maxSize)
    : _rect(position, size),
      _bgColor(bgColor),
      _fgColor(fgColor),
      _maxSize(maxSize)
{
    _text.text = "";
    _text.SetColor(fgColor);
    _text.SetFontSize(fgSize);
    _text.SetSpacing(fgSpacing);
}

std::string &TextEntry::text()
{
    return _text.text;
}

TextEntry::ClickedState TextEntry::isClicked(raylib::Window &window)
{
    if (!window.IsFocused()) {
        return ButtonNotPressed;
    }
    if (!raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
        return ButtonNotPressed;
    }
    const auto pos = raylib::Mouse::GetPosition();
    if (!_rect.CheckCollision(pos)) {
        return ClickedOutside;
    }
    return ClickedInside;
}

void TextEntry::update(raylib::Window &window)
{
    if (_isReadonly) {
        return;
    }
    switch (this->isClicked(window)) {
        case ClickedInside:
            if (_isFocused == false) {
                _isFocused = true;
            }
            break;
        case ClickedOutside:
            if (_isFocused == true) {
                _isFocused = false;
            }
            break;
        default: break;
    }
    if (!window.IsFocused() || !_isFocused) {
        return;
    }

    auto c = GetCharPressed();
    while (c != 0) {
        if ((c >= 32) && (c <= 125) && _text.text.length() < (size_t) _maxSize) {
            _text.text.push_back((char) c);
        }
        c = GetCharPressed();
    }
    if (raylib::Keyboard::IsKeyPressed(KEY_BACKSPACE)) {
        if (_text.text.length() > 0) {
            _text.text.erase(_text.text.length() - 1);
        }
    }
}

void TextEntry::draw(raylib::Window & /* unused */) const
{
    _rect.Draw(_bgColor);

    const auto size_text    = _text.MeasureEx();
    auto pos_text           = _rect.GetPosition();
    const auto texture_size = _rect.GetSize();
    pos_text.SetX(pos_text.GetX() + (texture_size.x / 2));
    pos_text.SetY(pos_text.GetY() + (texture_size.y / 2));
    pos_text.SetX(pos_text.GetX() - (size_text.GetX() / 2));
    pos_text.SetY(pos_text.GetY() - (size_text.GetY() / 2));
    _text.Draw(pos_text);
}

void TextEntry::setPosition(raylib::Vector2 position)
{
    _rect.SetPosition(position);
}

void TextEntry::setBgColor(const raylib::Color &color)
{
    _bgColor = color;
}

TextEntry &TextEntry::operator=(const TextEntry &src)
{
    _rect       = src._rect;
    _text.text  = src._text.text;
    _text.color = src._text.color;
    _bgColor    = src._bgColor;
    return *this;
}

const raylib::Rectangle &TextEntry::getRect() const
{
    return _rect;
}

void TextEntry::setReadonly(bool readOnly)
{
    _isReadonly = readOnly;
}
