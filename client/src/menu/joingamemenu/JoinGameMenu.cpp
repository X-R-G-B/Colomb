#include <memory>

#include "INetwork.hpp"
#include "JoinGameMenu.hpp"
#include "Logger.hpp"
#include "PathResolver.hpp"
#include "TextEntry.hpp"

JoinGameMenu::JoinGameMenu(raylib::Window &window)
{
    const auto win_size   = window.GetSize();
    const auto middle     = win_size.Divide(2);
    const auto mid_middle = middle.Divide(2);

    // ------------------- Join Room
    _textEntries["join_text"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::White(),
        raylib::Color::Black(),
        20,
        2.5,
        10);
    const auto sizeTextEntryJoinText = _textEntries["join_text"]->getRect().GetSize();
    _textEntries["join_text"]->setPosition(raylib::Vector2(
        middle.x - (sizeTextEntryJoinText.x / 2) - mid_middle.x,
        middle.y - sizeTextEntryJoinText.y));
    _textEntries["join_text"]->setReadonly(true);
    _textEntries["join_text"]->text().assign("Join Game:");
    // text entry roomName
    _textEntries["roomName"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5,
        10);
    const auto sizeTextEntryRoomName = _textEntries["roomName"]->getRect().GetSize();
    _textEntries["roomName"]->setPosition(raylib::Vector2(
        middle.x - (sizeTextEntryRoomName.x / 2) - mid_middle.x,
        middle.y + sizeTextEntryRoomName.y));
    // button connect
    _buttons["connecting"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/play.png"),
        "",
        true);
    const auto sizeButtonPlay = _buttons["connecting"]->getTexture().GetSize();
    _buttons["connecting"]->setPosition(raylib::Vector2(
        middle.x - (sizeButtonPlay.x / 2) - mid_middle.x,
        middle.y + sizeButtonPlay.y + sizeTextEntryRoomName.y));
    // ------------------- Create Room
    _textEntries["create_text"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::White(),
        raylib::Color::Black(),
        20,
        2.5,
        10);
    const auto sizeTextEntryCreateText = _textEntries["create_text"]->getRect().GetSize();
    _textEntries["create_text"]->setPosition(raylib::Vector2(
        middle.x - (sizeTextEntryCreateText.x / 2) + mid_middle.x,
        middle.y - sizeTextEntryCreateText.y));
    _textEntries["create_text"]->setReadonly(true);
    _textEntries["create_text"]->text().assign("Create Game:");
    // button connect
    _buttons["create"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/play.png"),
        "",
        true);
    const auto sizeButtonCreate = _buttons["create"]->getTexture().GetSize();
    _buttons["create"]->setPosition(raylib::Vector2(
        middle.x - (sizeButtonCreate.x / 2) + mid_middle.x,
        middle.y + sizeButtonCreate.y + sizeTextEntryCreateText.y));
}

void JoinGameMenu::update(raylib::Window &window)
{
    _textEntries["roomName"]->update(window);
    if (_buttons["connecting"]->isClicked(window)) {
        Logger::debug("TEST:" + _textEntries["roomName"]->text());
        network.send({
            {"type",     "join"                          },
            {"roomName", _textEntries["roomName"]->text()},
        });
    }
    if (_buttons["create"]->isClicked(window)) {
        Logger::debug("TEST: create");
        network.send({
            {"type", "create"},
        });
    }
}

void JoinGameMenu::draw(raylib::Window &window)
{
    _textEntries["roomName"]->draw(window);
    _buttons["connecting"]->draw(window);
    _textEntries["join_text"]->draw(window);
    _buttons["create"]->draw(window);
    _textEntries["create_text"]->draw(window);
}

void JoinGameMenu::free(raylib::Window &window)
{
}
