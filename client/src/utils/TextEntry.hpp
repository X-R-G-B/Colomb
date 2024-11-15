#pragma once

#include <string>
#include "raylib-cpp.hpp"

class TextEntry {
    public:
        enum ClickedState {
            ClickedOutside,
            ClickedInside,
            ButtonNotPressed,
        };
        TextEntry(
            raylib::Vector2 position,
            raylib::Vector2 size,
            raylib::Color bgColor = raylib::Color::Black(),
            raylib::Color fgColor = raylib::Color::White(),
            int fgSize            = 10,
            float fgSpacing       = 2,
            int maxSize           = 0);

        std::string &text();
        void update(raylib::Window &window);
        void draw(raylib::Window &window) const;
        void setPosition(raylib::Vector2 position);
        void setBgColor(const raylib::Color &color);
        const raylib::Rectangle &getRect() const;
        void setReadonly(bool readOnly);
        ClickedState isClicked(raylib::Window &wndow);
        void setRectSizeToTextSize();

        TextEntry &operator=(const TextEntry &src);

    private:
        bool _isReadonly = false;
        raylib::Rectangle _rect;
        raylib::Text _text;
        raylib::Color _bgColor;
        raylib::Color _fgColor;
        bool _isFocused = false;
        int _maxSize;
};
