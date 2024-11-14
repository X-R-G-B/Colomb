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
}

void Participants::free(raylib::Window &window)
{
}
