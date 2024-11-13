#include <memory>

#include "Logger.hpp"
#include "PathResolver.hpp"
#include "JoinGameMenu.hpp"
#include "TextEntry.hpp"

JoinGameMenu::JoinGameMenu(raylib::Window &window)
{
    // text entry roomName
    _textEntries["roomName"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0), raylib::Vector2(100, 50), raylib::Color::Black(), raylib::Color::White(), 20
    );
    const auto sizeTextEntryRoomName = _textEntries["roomName"]->getRect().GetSize();
    _textEntries["roomName"]->setPosition(raylib::Vector2(
        window.GetWidth() / 2.0 - sizeTextEntryRoomName.x / 2,
        (window.GetHeight() / 2.0 + sizeTextEntryRoomName.y)));
    // button connect
    _buttons["connecting"] =
        std::make_unique<Button>(raylib::Vector2(0, 0), PathResolver::resolve("assets/icons/play.png"), "", false);
    const auto sizeButtonPlay = _buttons["connecting"]->getTexture().GetSize();
    _buttons["connecting"]->setPosition(raylib::Vector2(
        window.GetWidth() / 2.0 - sizeButtonPlay.x / 2,
        (window.GetHeight() / 2.0 + sizeButtonPlay.y + sizeTextEntryRoomName.y)));
}

void JoinGameMenu::update(raylib::Window &window)
{
    _textEntries["roomName"]->update(window);
    if (_buttons["connecting"]->isClicked(window)) {
        Logger::debug("TEST:" + _textEntries["roomName"]->text());
    }
}

void JoinGameMenu::draw(raylib::Window &window)
{

    _textEntries["roomName"]->draw(window);
    _buttons["connecting"]->draw(window);
}

void JoinGameMenu::free(raylib::Window &window)
{
}
