#pragma once

#include <memory>
#include <string>
#include "IMenu.hpp"

class MenusManager : public IMenu {
    public:
        MenusManager(raylib::Window &window);
        void update(raylib::Window &window) override;
        void draw(raylib::Window &window) override;
        void free(raylib::Window &window) override;

    private:
        std::string _currentState;
        std::unique_ptr<IMenu> _menu;
};
