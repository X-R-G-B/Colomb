#include "GamePendingMenu.hpp"
#include <algorithm>
#include <memory>
#include "GlobalValues.hpp"
#include "INetwork.hpp"
#include "MenuState.hpp"
#include "PathResolver.hpp"
#include "TextEntry.hpp"

GamePendingMenu::GamePendingMenu(raylib::Window &window) : _participants(window), _gamesMode(window)
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
        2.5);
    const auto sizeTextRoomName = _textEntries["roomName"]->getRect().GetSize();
    _textEntries["roomName"]->setPosition(raylib::Vector2(middle.x - sizeTextRoomName.x / 2, 10));
    _textEntries["roomName"]->setReadonly(true);
    _textEntries["roomName"]->text().assign(globalValues._roomName);
    // ------------------------------ start
    // button start
    _textEntries["start_button"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5);
    const auto sizeTextStart = _textEntries["start_button"]->getRect().GetSize();
    _textEntries["start_button"]->setPosition(
        raylib::Vector2(middle.x - (sizeTextStart.x / 2), middle.y + mid_middle.y - (sizeTextStart.y / 2)));
    _textEntries["start_button"]->setReadonly(true);
    _textEntries["start_button"]->text().assign("Start!");
    // ------------------------------ ready
    // text display ready
    _textEntries["ready_state"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5);
    const auto sizeTextReady = _textEntries["ready_state"]->getRect().GetSize();
    _textEntries["ready_state"]->setPosition(raylib::Vector2(
        middle.x - (sizeTextReady.x / 2),
        _textEntries["start_button"]->getRect().GetPosition().y + sizeTextStart.y + 10));
    _textEntries["ready_state"]->setReadonly(true);
    _textEntries["ready_state"]->text().assign("Ready:");
    // button ready ok
    _buttons["ready_ok"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/thumb-up.png"),
        "",
        _isReady);
    _buttons["ready_ok"]->setPosition(raylib::Vector2(
        _textEntries["ready_state"]->getRect().GetPosition().x,
        _textEntries["ready_state"]->getRect().GetPosition().y
            + _textEntries["ready_state"]->getRect().GetSize().y + 5));
    // button ready not ok
    _buttons["ready_ko"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/thumb-down.png"),
        "",
        !_isReady);
    _buttons["ready_ko"]->setPosition(raylib::Vector2(
        _textEntries["ready_state"]->getRect().GetPosition().x,
        _textEntries["ready_state"]->getRect().GetPosition().y
            + _textEntries["ready_state"]->getRect().GetSize().y + 10));
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
            const auto participants       = message.at("players").template get<std::vector<std::string>>();
            const auto participants_ready = message.at("players_ready").template get<std::vector<bool>>();
            const auto owner              = message.at("owner").template get<std::string>();
            const auto game               = message.at("game").template get<std::string>();
            _gamesMode.setCurrentGameMode(game);
            _participants.clearParticipants();
            for (size_t i = 0; i < std::min(participants.size(), participants_ready.size()); i++) {
                _participants.addParticipant(window, participants[i]);
                _participants.setParticipantReady(participants[i], participants_ready[i]);
            }
            _participants.setOwner(owner);
        } else if (messageType == "newPlayer") {
            if (!message.contains("player") || !message.at("player").is_string()) {
                continue;
            }
            const auto player = message.at("player").template get<std::string>();
            _participants.addParticipant(window, player);
        } else if (messageType == "delPlayer") {
            if (!message.contains("player") || !message.at("player").is_string()) {
                continue;
            }
            const auto player = message.at("player").template get<std::string>();
            _participants.removeParticipant(player);
        } else if (messageType == "ready") {
            if (!message.contains("player") || !message.contains("ready")
                || !message.at("player").is_string() || !message.at("ready").is_boolean()) {
                continue;
            }
            const auto player = message.at("player").template get<std::string>();
            const auto ready  = message.at("ready").template get<bool>();
            _participants.setParticipantReady(player, ready);
            if (player == globalValues._username) {
                _isReady = ready;
                _buttons["ready_ko"]->setClickable(!_isReady);
                _buttons["ready_ok"]->setClickable(_isReady);
            }
        } else if (messageType == "games") {
            if (!message.contains("games_name") || !message.contains("games_description")
                || !message.at("games_name").is_array() || !message.at("games_description").is_array()) {
                continue;
            }
            const auto games_name = message.at("games_name").template get<std::vector<std::string>>();
            const auto games_description =
                message.at("games_description").template get<std::vector<std::string>>();
            // TODO:
        } else if (messageType == "select") {
            if (!message.contains("gameName") || !message.at("gameName").is_string()) {
                continue;
            }
            const auto game = message.at("gameName").template get<std::string>();
            if (message.contains("success")) {
                if (message.at("success").is_boolean()) {
                    const auto success = message.at("success").template get<bool>();
                    if (!success) {
                        // TODO: notif error
                        continue;
                    }
                } else {
                    continue;
                }
            }
            _gamesMode.setCurrentGameMode(game);
        } else if (messageType == "start") {
            if (!message.contains("gameName") || !message.at("gameName").is_string()) {
                continue;
            }
            const auto game = message.at("gameName").template get<std::string>();
            if (message.contains("success")) {
                if (message.at("success").is_boolean()) {
                    const auto success = message.at("success").template get<bool>();
                    if (!success) {
                        // TODO: notif error
                        continue;
                    }
                } else {
                    continue;
                }
            }
            menuState.setState(M_GAMEMENU);
            return;
        }
    }
    if (_textEntries["roomName"]->isClicked(window)) {
        window.SetClipboardText(_textEntries["roomName"]->text());
    }
    if (_buttons["ready_ko"]->isClicked(window)) {
        network.send({
            {"type",  "ready"},
            {"ready", true   },
        });
        _isReady = true;
        _buttons["ready_ko"]->setClickable(!_isReady);
        _buttons["ready_ok"]->setClickable(_isReady);
    } else if (_buttons["ready_ok"]->isClicked(window)) {
        network.send({
            {"type",  "ready"},
            {"ready", false  },
        });
        _isReady = false;
        _buttons["ready_ko"]->setClickable(!_isReady);
        _buttons["ready_ok"]->setClickable(_isReady);
    }
    _participants.update(window);
    _gamesMode.update(window);
    if (_participants.getOwner() == globalValues._username
        && _textEntries["start_button"]->isClicked(window) == TextEntry::ClickedInside) {
        network.send({
            {"type", "start"}
        });
    }
}

void GamePendingMenu::draw(raylib::Window &window)
{
    _textEntries["roomName"]->draw(window);
    _textEntries["ready_state"]->draw(window);
    if (_isReady) {
        _buttons["ready_ok"]->draw(window);
    } else {
        _buttons["ready_ko"]->draw(window);
    }
    if (_participants.getOwner() == globalValues._username) {
        _textEntries["start_button"]->draw(window);
    }
    _participants.draw(window);
    _gamesMode.draw(window);
}

void GamePendingMenu::free(raylib::Window &window)
{
    _participants.free(window);
    _gamesMode.free(window);
}
