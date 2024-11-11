#include "FirstMenu.hpp"
#include <algorithm>
#include <memory>
#include "Logger.hpp"
#include "PathResolver.hpp"

FirstMenu::FirstMenu(raylib::Window & /*unused*/) : _icon(PathResolver::resolve("assets/icons/favicon.png"))
{
    _buttons["ok"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/house.png"),
        "test");
}

void FirstMenu::update(raylib::Window &window)
{
    for (const auto &button : _buttons) {
        if (button.first == "ok" && button.second->isClicked(window)) {
            Logger::error("TEST");
        }
    }
}

void FirstMenu::draw(raylib::Window &window)
{
    _icon.Draw(
        window.GetWidth() / 2 - FirstMenu::_icon.GetWidth() / 2,
        window.GetHeight() / 2 - FirstMenu::_icon.GetHeight() / 2);
    for (const auto &button : _buttons) {
        button.second->draw(window);
    }
}

void FirstMenu::free(raylib::Window & /*unused*/)
{
}
