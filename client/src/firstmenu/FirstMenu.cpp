#include "FirstMenu.hpp"
#include "PathResolver.hpp"

FirstMenu::FirstMenu(raylib::Window & /*unused*/) : _icon(PathResolver::resolve("assets/icons/favicon.png"))
{
}

void FirstMenu::update(raylib::Window & /*unused*/)
{
}

void FirstMenu::draw(raylib::Window &window)
{
    FirstMenu::_icon.Draw(
        window.GetWidth() / 2 - FirstMenu::_icon.GetWidth() / 2,
        window.GetHeight() / 2 - FirstMenu::_icon.GetHeight() / 2);
}

void FirstMenu::free(raylib::Window & /*unused*/)
{
}
