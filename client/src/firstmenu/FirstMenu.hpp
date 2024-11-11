#pragma once

#include "raylib-cpp.hpp"

class FirstMenu {
    public:
        FirstMenu(raylib::Window &window);
        void update(raylib::Window &window);
        void draw(raylib::Window &window);
        void free(raylib::Window &window);

    private:
        raylib::Texture _icon;
};
