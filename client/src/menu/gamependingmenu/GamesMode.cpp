#include "GamesMode.hpp"
#include <memory>
#include "PathResolver.hpp"

GamesMode::GamesMode(raylib::Window &window)
{
    const auto win_size   = window.GetSize();
    const auto middle     = win_size.Divide(2);
    const auto mid_middle = middle.Divide(2);

    // text display game
    _textEntries["game_text"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5);
    const auto sizeTextGame = _textEntries["game_text"]->getRect().GetSize();
    _textEntries["game_text"]->setPosition(
        raylib::Vector2(middle.x + mid_middle.x - (sizeTextGame.x / 2), 10));
    _textEntries["game_text"]->setReadonly(true);
    _textEntries["game_text"]->text().assign("Game:");
    // text display game name
    _textEntries["game_name"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5);
    _textEntries["game_name"]->setPosition(
        raylib::Vector2(_textEntries["game_text"]->getRect().GetPosition().x, 10 + sizeTextGame.y + 10));
    _textEntries["game_name"]->setReadonly(true);
    // arrow up
    _buttons["arrow_up"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/arrow-up.png"),
        "",
        true);
    _buttons["arrow_up"]->setPosition(raylib::Vector2(
        _textEntries["game_name"]->getRect().GetX() + 10,
        _textEntries["game_name"]->getRect().GetY() + _textEntries["game_name"]->getRect().GetSize().GetY()
            + 5));
    // arrow down
    _buttons["arrow_down"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/arrow-down.png"),
        "",
        true);
    _buttons["arrow_down"]->setPosition(
        raylib::Vector2(_textEntries["game_name"]->getRect().GetX() + 10, middle.y + mid_middle.y));
}

void GamesMode::update(raylib::Window &window)
{
    if (_buttons["arrow_up"]->isClicked(window)) {
        for (auto &[_, textEntry] : _textGamesMode) {
            const auto size_box = textEntry->getRect().GetSize().y + 5;
            textEntry->setPosition(raylib::Vector2(
                textEntry->getRect().GetPosition().GetX(),
                textEntry->getRect().GetPosition().GetY() - size_box));
        }
    } else if (_buttons["arrow_down"]->isClicked(window)) {
        for (auto &[_, textEntry] : _textGamesMode) {
            const auto size_box = textEntry->getRect().GetSize().y + 5;
            textEntry->setPosition(raylib::Vector2(
                textEntry->getRect().GetPosition().GetX(),
                textEntry->getRect().GetPosition().GetY() + size_box));
        }
    }
    for (const auto &[gameModeKey, textEntry] : _textGamesMode) {
        if (textEntry->isClicked(window) == TextEntry::ClickedInside) {
            _onGameModeClicked(gameModeKey);
        }
    }
}

void GamesMode::draw(raylib::Window &window)
{
    _textEntries["game_name"]->draw(window);
    _textEntries["game_text"]->draw(window);
    const auto min_y =
        _buttons["arrow_up"]->GetPosition().y + _buttons["arrow_up"]->getTexture().GetSize().y + 5;
    const auto max_y = _buttons["arrow_down"]->GetPosition().y - 5;
    for (const auto &gameMode : _gamesMode) {
        if (_textGamesMode.contains(gameMode)
            && _textGamesMode[gameMode]->getRect().GetPosition().y
                    + _textGamesMode[gameMode]->getRect().GetSize().y
                < max_y
            && _textGamesMode[gameMode]->getRect().GetPosition().y > min_y) {
            _textGamesMode[gameMode]->draw(window);
        }
    }
    _buttons["arrow_up"]->draw(window);
    _buttons["arrow_down"]->draw(window);
}

void GamesMode::free(raylib::Window & /* unused */)
{
}

void GamesMode::addGamesMode(const std::string &gameMode)
{
    raylib::Vector2 cursor(0, 0);

    if (_textGamesMode.contains(gameMode)) {
        return;
    }
    if (_gamesMode.size() == 0) {
        cursor.x = _textEntries["game_name"]->getRect().GetX();
        cursor.y = _buttons["arrow_up"]->GetPosition().GetY()
            + _buttons["arrow_up"]->getTexture().GetSize().y + 10;
    } else {
        cursor = _textGamesMode[_gamesMode.back()]->getRect().GetPosition()
            + raylib::Vector2(0, _textGamesMode[_gamesMode.back()]->getRect().GetSize().y + 5);
    }
    // gameMode
    _textGamesMode[gameMode] = std::make_unique<TextEntry>(
        cursor,
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5,
        15);
    _textGamesMode[gameMode]->setReadonly(true);
    _textGamesMode[gameMode]->text().assign(gameMode);
    _gamesMode.push_back(gameMode);
}

void GamesMode::clearGamesMode()
{
    _gamesMode.clear();
    _textGamesMode.clear();
    _currentGameMode = "";
}

void GamesMode::setCurrentGameMode(const std::string &gameMode)
{
    _currentGameMode = gameMode;
    _textEntries["game_name"]->text().assign(gameMode);
    _textEntries["game_name"]->setRectSizeToTextSize();
}

const std::string &GamesMode::getCurrentGameMode() const
{
    return _currentGameMode;
}

void GamesMode::setOnGameModeClicked(OnGameModeClicked onGameModeClicked)
{
    _onGameModeClicked = onGameModeClicked;
}
