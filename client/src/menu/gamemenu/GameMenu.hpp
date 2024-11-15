#pragma once

#include "raylib-cpp.hpp"
#include "IMenu.hpp"

class GameMenu : public IMenu {
    public:
        GameMenu(raylib::Window &window);
        void update(raylib::Window &window);
        void draw(raylib::Window &window);
        void free(raylib::Window &window);

    private:
};