#pragma once

#include <memory>
#include <unordered_map>
#include "raylib-cpp.hpp"
#include "Button.hpp"

class FirstMenu {
    public:
        FirstMenu(raylib::Window &window);
        void update(raylib::Window &window);
        void draw(raylib::Window &window);
        void free(raylib::Window &window);

    private:
        raylib::Texture _icon;
        std::unordered_map<std::string, std::unique_ptr<Button>> _buttons;
};
