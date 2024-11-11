#pragma once

#include <list>
#include <string>
#include <vector>
#include "raylib-cpp.hpp"

class Button {
    public:
        Button(raylib::Vector2 position, std::string texture, std::string text);

        bool isClicked(raylib::Window &window) const;
        void draw(raylib::Window &window) const;

        Button &operator=(const Button &src);

    private:
        raylib::Vector2 _position;
        raylib::Texture _texture;
        raylib::Text _text;
};
