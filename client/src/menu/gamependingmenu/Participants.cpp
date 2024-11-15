#include "Participants.hpp"
#include <algorithm>
#include <memory>
#include "PathResolver.hpp"

Participants::Participants(raylib::Window &window)
{
    const auto win_size   = window.GetSize();
    const auto middle     = win_size.Divide(2);
    const auto mid_middle = middle.Divide(2);

    // text display participant
    _textEntries["participants_text"] = std::make_unique<TextEntry>(
        raylib::Vector2(0, 0),
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5,
        10);
    const auto sizeTextEntryParticipants = _textEntries["participants_text"]->getRect().GetSize();
    _textEntries["participants_text"]->setPosition(
        raylib::Vector2(mid_middle.x - sizeTextEntryParticipants.x / 2, 10));
    _textEntries["participants_text"]->setReadonly(true);
    _textEntries["participants_text"]->text().assign("Players:");
    // crown for owner
    _buttons["crown"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/crown.png"),
        "",
        false);
    // arrow up
    _buttons["arrow_up"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/arrow-up.png"),
        "",
        true);
    _buttons["arrow_up"]->setPosition(raylib::Vector2(
        _textEntries["participants_text"]->getRect().GetX() + 10,
        _textEntries["participants_text"]->getRect().GetY()
            + _textEntries["participants_text"]->getRect().GetSize().GetY() + 5));
    // arrow down
    _buttons["arrow_down"] = std::make_unique<Button>(
        raylib::Vector2(0, 0),
        PathResolver::resolve("assets/icons/arrow-down.png"),
        "",
        true);
    _buttons["arrow_down"]->setPosition(
        raylib::Vector2(_textEntries["participants_text"]->getRect().GetX() + 10, middle.y + mid_middle.y));
}

void Participants::update(raylib::Window &window)
{
    if (_buttons["arrow_up"]->isClicked(window)) {
        for (auto &[_, textEntry] : _textUsernames) {
            const auto size_box = textEntry->getRect().GetSize().y + 5;
            textEntry->setPosition(raylib::Vector2(
                textEntry->getRect().GetPosition().GetX(),
                textEntry->getRect().GetPosition().GetY() - size_box));
        }
        _ownerSet = false;
    } else if (_buttons["arrow_down"]->isClicked(window)) {
        for (auto &[_, textEntry] : _textUsernames) {
            const auto size_box = textEntry->getRect().GetSize().y + 5;
            textEntry->setPosition(raylib::Vector2(
                textEntry->getRect().GetPosition().GetX(),
                textEntry->getRect().GetPosition().GetY() + size_box));
        }
        _ownerSet = false;
    }
    if (_ownerSet == false && _owner.length() != 0 && _textUsernames.contains(_owner)) {
        auto posOwner     = _textUsernames[_owner]->getRect().GetPosition();
        const auto size_x = _buttons["crown"]->getTexture().GetWidth();
        posOwner.x -= (size_x + 5);
        _buttons["crown"]->setPosition(posOwner);
        _ownerSet = true;
    }
}

void Participants::draw(raylib::Window &window)
{
    _textEntries["participants_text"]->draw(window);
    const auto min_y =
        _buttons["arrow_up"]->GetPosition().y + _buttons["arrow_up"]->getTexture().GetSize().y + 5;
    const auto max_y = _buttons["arrow_down"]->GetPosition().y - 5;
    for (const auto &user : _usernames) {
        if (_buttonsUsername.contains(user)
            && _buttonsUsername[user]->GetPosition().y + _buttonsUsername[user]->getTexture().GetSize().y
                < max_y
            && _buttonsUsername[user]->GetPosition().y > min_y) {
            _buttonsUsername[user]->draw(window);
        }
        if (_textUsernames.contains(user)
            && _textUsernames[user]->getRect().GetPosition().y + _textUsernames[user]->getRect().GetSize().y
                < max_y
            && _textUsernames[user]->getRect().GetPosition().y > min_y) {
            _textUsernames[user]->draw(window);
            if (user == _owner && _ownerSet) {
                _buttons["crown"]->draw(window);
            }
        }
    }
    _buttons["arrow_up"]->draw(window);
    _buttons["arrow_down"]->draw(window);
}

void Participants::free(raylib::Window & /* unused */)
{
}

void Participants::addParticipant(raylib::Window & /* unused */, const std::string &username)
{
    raylib::Vector2 cursor(0, 0);

    if (_textUsernames.contains(username)) {
        return;
    }
    if (_usernames.size() == 0) {
        cursor.x = _textEntries["participants_text"]->getRect().GetX();
        cursor.y = _textEntries["participants_text"]->getRect().GetY()
            + _textEntries["participants_text"]->getRect().GetSize().GetY() + 5
            + _buttons["arrow_up"]->getTexture().GetSize().y + 10;
    } else {
        cursor = _textUsernames[_usernames.back()]->getRect().GetPosition()
            + raylib::Vector2(0, _textUsernames[_usernames.back()]->getRect().GetSize().y + 5);
    }
    // username
    _textUsernames[username] = std::make_unique<TextEntry>(
        cursor,
        raylib::Vector2(100, 50),
        raylib::Color::Black(),
        raylib::Color::White(),
        20,
        2.5,
        15);
    _textUsernames[username]->setReadonly(true);
    _textUsernames[username]->text().assign(username);
    _usernames.push_back(username);
    _ownerSet = false;
}

void Participants::removeParticipant(const std::string &username)
{
    int index = -1;
    for (int i = 0; i < static_cast<int>(_usernames.size()); i++) {
        if (_usernames[i] == username) {
            index = i;
            break;
        }
    }
    if (index > 0 && index < (int) _usernames.size()) {
        const auto size_box = _textUsernames[username]->getRect().GetSize().y + 5;
        std::for_each(
            _usernames.begin() + index + 1,
            _usernames.end(),
            [this, size_box](const std::string &user_i) {
                _textUsernames[user_i]->setPosition(raylib::Vector2(
                    _textUsernames[user_i]->getRect().GetPosition().GetX(),
                    _textUsernames[user_i]->getRect().GetPosition().GetY() - size_box));
            });
        _textUsernames.erase(username);
        _buttonsUsername.erase(username);
        _usernames.erase(_usernames.begin() + index);
    }
    _ownerSet = false;
}

void Participants::clearParticipants()
{
    _usernames.clear();
    _textUsernames.clear();
    _buttonsUsername.clear();
    _owner    = "";
    _ownerSet = false;
}

void Participants::setOwner(const std::string &username)
{
    _owner    = username;
    _ownerSet = false;
}

void Participants::setParticipantReady(const std::string &username, bool ready)
{
    if (!_textUsernames.contains(username)) {
        return;
    }
    if (ready) {
        _textUsernames[username]->setBgColor(raylib::Color::DarkGreen());
    } else {
        _textUsernames[username]->setBgColor(raylib::Color::DarkBrown());
    }
}

const std::string &Participants::getOwner() const
{
    return _owner;
}
