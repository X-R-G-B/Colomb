#pragma once

#include <string>
#include "raylib-cpp.hpp"

class TextEntry {
    public:
        TextEntry(raylib::Vector2 position, raylib::Vector2 size, raylib::Color bgColor, raylib::Color fgColor, int fgSize);

        std::string &text();
        void update(raylib::Window &window);
        void draw(raylib::Window &window) const;
        void setPosition(raylib::Vector2 position);
        const raylib::Rectangle &getRect() const;

        TextEntry &operator=(const TextEntry &src);

    private:
        raylib::Rectangle _rect;
        raylib::Text _text;
        raylib::Color _bgColor;
        raylib::Color _fgColor;
        bool _isFocused = false;
};
