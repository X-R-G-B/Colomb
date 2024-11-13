#pragma once

#include <string>
#define menuState (MenuState::GetInstance())

class MenuState {
    public:
        static MenuState &GetInstance();

        MenuState &operator=(const MenuState &MenuState) = delete;
        MenuState &operator=(const MenuState MenuState)  = delete;

        void setState(const std::string &state);
        const std::string &getState() const;

    private:
        MenuState();
        std::string _state = "firstmenu";
};
