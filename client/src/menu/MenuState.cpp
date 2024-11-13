#include "MenuState.hpp"

MenuState::MenuState()
{
}

MenuState &MenuState::GetInstance()
{
    static MenuState menuStateInternal;
    return menuStateInternal;
}

void MenuState::setState(const std::string &state)
{
    _state = state;
}

const std::string &MenuState::getState() const
{
    return _state;
}
