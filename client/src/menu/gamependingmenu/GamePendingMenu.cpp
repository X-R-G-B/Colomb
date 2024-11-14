#include "GamePendingMenu.hpp"
#include <memory>
#include "GlobalValues.hpp"

GamePendingMenu::GamePendingMenu(raylib::Window &window): _participants(window)
{
    const auto win_size   = window.GetSize();
    const auto middle     = win_size.Divide(2);
    const auto mid_middle = middle.Divide(2);

    // text display roomName
    _textEntries["roomName"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5,
        10);
    const auto sizeTextEntryRoomName = _textEntries["roomName"]->getRect().GetSize();
    _textEntries["roomName"]->setPosition(
        raylib::Vector2(middle.x - sizeTextEntryRoomName.x / 2, 0 + sizeTextEntryRoomName.y / 2));
    _textEntries["roomName"]->setReadonly(true);
    _textEntries["roomName"]->text().assign(globalValues._roomName);
}

void GamePendingMenu::update(raylib::Window &window)
{
    if (_textEntries["roomName"]->isClicked(window)) {
        window.SetClipboardText(_textEntries["roomName"]->text());
    }
    _participants.update(window);
}

void GamePendingMenu::draw(raylib::Window &window)
{
    _textEntries["roomName"]->draw(window);
    _participants.draw(window);
}

void GamePendingMenu::free(raylib::Window &window)
{
    _participants.free(window);
}
