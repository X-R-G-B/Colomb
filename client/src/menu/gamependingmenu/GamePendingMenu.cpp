#include "GamePendingMenu.hpp"
#include <memory>
#include "GlobalValues.hpp"
#include "INetwork.hpp"
#include "Logger.hpp"

GamePendingMenu::GamePendingMenu(raylib::Window &window) : _participants(window)
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
    if (!_stateFirstSync) {
        network.send({
            {"type", "state"}
        });
        _stateFirstSync = true;
    }
    while (network.hasPacket()) {
        const auto message = network.receive();
        if (!message.contains("type") || !message.at("type").is_string()) {
            continue;
        }
        const auto messageType = message.at("type").template get<std::string>();
        if (messageType == "state") {
            if (!message.contains("players") || !message.contains("owner") || !message.contains("game")
                || !message.at("players").is_array() || !message.at("owner").is_string()
                || !message.at("game").is_string()) {
                continue;
            }
            const auto participants = message.at("players").template get<std::vector<std::string>>();
            const auto owner        = message.at("owner").template get<std::string>();
            const auto game         = message.at("game").template get<std::string>();
            _participants.clearParticipants();
            for (const auto &username : participants) {
                _participants.addParticipant(window, username);
            }
        } else if (messageType == "newPlayer") {
            if (!message.contains("player") || !message.at("player").is_string()) {
                continue;
            }
            const auto player = message.at("player").template get<std::string>();
            Logger::debug("NEWPLAYER: " + player);
            _participants.addParticipant(window, player);
        } else if (messageType == "delPlayer") {
            if (!message.contains("player") || !message.at("player").is_string()) {
                continue;
            }
            const auto player = message.at("player").template get<std::string>();
            _participants.removeParticipant(player);
        }
    }
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
