#include "Participants.hpp"

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
        raylib::Vector2(0 + sizeTextEntryParticipants.x / 2, 0 + sizeTextEntryParticipants.y / 2));
    _textEntries["participants_text"]->setReadonly(true);
    _textEntries["participants_text"]->text().assign("Players:");
}

void Participants::update(raylib::Window &window)
{
}

void Participants::draw(raylib::Window &window)
{
    _textEntries["participants_text"]->draw(window);
    for (const auto &user : _usernames) {
        if (_buttonsUsername.contains(user)) {
            _buttonsUsername[user]->draw(window);
        }
        if (_textUsernames.contains(user)) {
            _textUsernames[user]->draw(window);
        }
    }
}

void Participants::free(raylib::Window &window)
{
}

void Participants::addParticipant(const std::string &username)
{
}

void Participants::removeParticipant(const std::string &username)
{
    _textUsernames.erase(username);
    _buttonsUsername.erase(username);
    int index = -1;
    for (int i = 0; i < static_cast<int>(_usernames.size()); i++) {
        if (_usernames[i] == username) {
            index = i;
            break;
        }
    }
    if (index > 0 && index < (int) _usernames.size()) {
        _usernames.erase(_usernames.begin() + index);
    }
}

void Participants::clearParticipants(const std::string &username)
{
    _usernames.clear();
    _textUsernames.clear();
    _buttonsUsername.clear();
}
