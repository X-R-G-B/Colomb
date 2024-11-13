#include "MenusManager.hpp"
#include <memory>
#include "FirstMenu.hpp"
#include "JoinGameMenu.hpp"
#include "IMenu.hpp"
#include "MenuState.hpp"

static std::unique_ptr<IMenu> create_with_state(raylib::Window &window)
{
    const auto state = menuState.getState();
    if (state == "firstmenu") {
        return std::make_unique<FirstMenu>(window);
    } else if (state == "joingamemenu") {
        return std::make_unique<JoinGameMenu>(window);
    } else {
        return std::make_unique<FirstMenu>(window);
    }
}

MenusManager::MenusManager(raylib::Window &window): _currentState(menuState.getState()), _menu(create_with_state(window))
{
}

void MenusManager::update(raylib::Window &window)
{
    if (_currentState != menuState.getState()) {
        _menu->free(window);
        _menu = create_with_state(window);
        _currentState = menuState.getState();
    }
    _menu->update(window);
}


void MenusManager::draw(raylib::Window &window)
{
    _menu->draw(window);
}

void MenusManager::free(raylib::Window &window)
{
    _menu->free(window);
}
