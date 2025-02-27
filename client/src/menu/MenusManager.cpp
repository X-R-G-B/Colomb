#include "MenusManager.hpp"
#include <memory>
#include "FirstMenu.hpp"
#include "GameMenu.hpp"
#include "GamePendingMenu.hpp"
#include "IMenu.hpp"
#include "JoinGameMenu.hpp"
#include "MenuState.hpp"

static std::unique_ptr<IMenu> create_with_state(raylib::Window &window)
{
    const auto state = menuState.getState();
    if (state == M_FIRSTMENU) {
        return std::make_unique<FirstMenu>(window);
    } else if (state == M_JOINGAMEMENU) {
        return std::make_unique<JoinGameMenu>(window);
    } else if (state == M_GAMEPENDIGMENU) {
        return std::make_unique<GamePendingMenu>(window);
    } else if (state == M_GAMEMENU) {
        return std::make_unique<GameMenu>(window);
    } else {
        return std::make_unique<FirstMenu>(window);
    }
}

MenusManager::MenusManager(raylib::Window &window)
    : _currentState(menuState.getState()),
      _menu(create_with_state(window))
{
}

void MenusManager::update(raylib::Window &window)
{
    if (_currentState != menuState.getState()) {
        _menu->free(window);
        _menu         = create_with_state(window);
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
