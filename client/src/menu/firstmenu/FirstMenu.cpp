#include "FirstMenu.hpp"
#include <memory>
#include "INetwork.hpp"
#include "Logger.hpp"
#include "MenuState.hpp"
#include "PathResolver.hpp"

FirstMenu::FirstMenu(raylib::Window &window) : _icon(PathResolver::resolve("assets/icons/favicon.png"))
{
    // button play
    _buttons["play"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/play.png"),
        "",
        false);
    const auto sizeButtonPlay = _buttons["play"]->getTexture().GetSize();
    _buttons["play"]->setPosition(raylib::Vector2(
        window.GetWidth() / 2.0 - sizeButtonPlay.x / 2,
        (window.GetHeight() / 2.0 + sizeButtonPlay.y)));
    // button charging
    _buttons["charging"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/cycle.png"),
        "",
        false);
    const auto sizeButtonCharging = _buttons["charging"]->getTexture().GetSize();
    _buttons["charging"]->setPosition(raylib::Vector2(
        window.GetWidth() / 2.0 - sizeButtonCharging.x / 2,
        (window.GetHeight() / 2.0 + sizeButtonCharging.y)));
}

void FirstMenu::update(raylib::Window &window)
{
    if (network.isConnected()) {
        _buttons["play"]->setClickable(true);
    }
    if (_buttons["play"]->isClicked(window)) {
        menuState.setState(M_JOINGAMEMENU);
    }
}

void FirstMenu::draw(raylib::Window &window)
{
    _icon.Draw(
        window.GetWidth() / 2 - FirstMenu::_icon.GetWidth() / 2,
        window.GetHeight() / 2 - FirstMenu::_icon.GetHeight() / 2);
    if (network.isConnected()) {
        _buttons["play"]->draw(window);
    } else {
        _buttons["charging"]->draw(window);
    }
}

void FirstMenu::free(raylib::Window & /*unused*/)
{
}
