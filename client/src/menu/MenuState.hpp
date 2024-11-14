#pragma once

#include <string>
#define menuState (MenuState::GetInstance())

#define M_GAMEMENU       "gamemenu"
#define M_FIRSTMENU      "firstmenu"
#define M_JOINGAMEMENU   "joingamemenu"
#define M_GAMEPENDIGMENU "gamependingmenu"

class MenuState {
    public:
        static MenuState &GetInstance();

        MenuState &operator=(const MenuState &MenuState) = delete;
        MenuState &operator=(const MenuState MenuState)  = delete;

        void setState(const std::string &state);
        const std::string &getState() const;

    private:
        MenuState();
        std::string _state = M_FIRSTMENU;
};
