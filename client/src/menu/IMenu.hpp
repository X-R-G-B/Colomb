#pragma once

#include "raylib-cpp.hpp"

class IMenu {
    public:
        virtual void update(raylib::Window &window) = 0;
        virtual void draw(raylib::Window &window) = 0;
        virtual void free(raylib::Window &window) = 0;
};
