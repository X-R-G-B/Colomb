#include <memory>

#include "GlobalValues.hpp"
#include "INetwork.hpp"
#include "JoinGameMenu.hpp"
#include "Logger.hpp"
#include "MenuState.hpp"
#include "PathResolver.hpp"
#include "TextEntry.hpp"

JoinGameMenu::JoinGameMenu(raylib::Window &window)
{
    const auto win_size   = window.GetSize();
    const auto middle     = win_size.Divide(2);
    const auto mid_middle = middle.Divide(2);

    // ------ username
    // text
    _textEntries["username_text"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::White(),
        raylib::Color::Black(),
        20,
        2.5,
        10);
    const auto sizeTextUsername = _textEntries["username_text"]->getRect().GetSize();
    _textEntries["username_text"]->setPosition(raylib::Vector2(
        middle.x - sizeTextUsername.x / 2,
        middle.y - (sizeTextUsername.y / 2) - mid_middle.y));
    _textEntries["username_text"]->setReadonly(true);
    _textEntries["username_text"]->text().assign("Username:");
    // entry
    _textEntries["username"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5,
        15);
    const auto sizeTextEntryUsername = _textEntries["username"]->getRect().GetSize();
    _textEntries["username"]->setPosition(raylib::Vector2(
        middle.x - sizeTextEntryUsername.x / 2,
        middle.y - mid_middle.y + (sizeTextUsername.y / 2) + 10));
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
    _textEntries["username"]->update(window);
    if (network.hasPacket()) {
        const auto message     = network.receive();
        const auto messageType = message.at("type").template get<std::string>();
        if (messageType == "join") {
            const auto success = message.at("success").template get<bool>();
            if (success) {
                const auto roomName = message.at("roomName").template get<std::string>();
                menuState.setState(M_GAMEPENDIGMENU);
                globalValues._roomName = roomName;
                return;
            } else {
                Logger::error("JOIN: got a success=false");
                // TODO: show notif
            }
        } else if (messageType == "create") {
            const auto success = message.at("success").template get<bool>();
            if (success) {
                const auto roomName    = message.at("roomName").template get<std::string>();
                globalValues._roomName = roomName;
                menuState.setState(M_GAMEPENDIGMENU);
                return;
            } else {
                Logger::error("CREATE: got a success=false");
                // TODO: show notif
            }
        }
    }
    if (_buttons["connecting"]->isClicked(window)) {
        if (_textEntries["username"]->text().length() != 0) {
            _textEntries["username"]->setBgColor(raylib::Color::Black());
            network.send({
                {"type",     "join"                          },
                {"username", _textEntries["username"]->text()},
                {"roomName", _textEntries["roomName"]->text()},
            });
        } else {
            _textEntries["username"]->setBgColor(raylib::Color::DarkGray());
        }
    }
    if (_buttons["create"]->isClicked(window)) {
        if (_textEntries["username"]->text().length() != 0) {
            _textEntries["username"]->setBgColor(raylib::Color::Black());
            network.send({
                {"type",     "create"                        },
                {"username", _textEntries["username"]->text()},
            });
        } else {
            _textEntries["username"]->setBgColor(raylib::Color::DarkGray());
        }
    }
}

void JoinGameMenu::draw(raylib::Window &window)
{
    _textEntries["roomName"]->draw(window);
    _buttons["connecting"]->draw(window);
    _textEntries["join_text"]->draw(window);
    _buttons["create"]->draw(window);
    _textEntries["create_text"]->draw(window);
    _textEntries["username_text"]->draw(window);
    _textEntries["username"]->draw(window);
}

void JoinGameMenu::free(raylib::Window &window)
{
}
