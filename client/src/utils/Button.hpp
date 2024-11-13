#pragma once

#include <string>
#include "raylib-cpp.hpp"

class Button {
    public:
        Button(raylib::Vector2 position, const std::string &texture, const std::string &text, bool clickable);

        bool isClicked(raylib::Window &window) const;
        void draw(raylib::Window &window) const;
        void setPosition(raylib::Vector2 position);
        const raylib::Texture &getTexture() const;
        void setClickable(bool clickable);

        Button &operator=(const Button &src);

    private:
        raylib::Vector2 _position;
        raylib::Texture _texture;
        raylib::Text _text;
        bool _clickable;
};
