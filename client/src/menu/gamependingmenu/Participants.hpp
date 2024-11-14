#pragma once

#include <memory>
#include <unordered_map>
#include "raylib-cpp.hpp"
#include "Button.hpp"
#include "TextEntry.hpp"

class Participants {
    public:
        Participants(raylib::Window &window);

        void update(raylib::Window &window);
        void draw(raylib::Window &window);
        void free(raylib::Window &window);

        void addParticipant(const std::string &username);
        void clearParticipants(const std::string &username);
        void removeParticipant(const std::string &username);

    private:
        std::unordered_map<std::string, std::unique_ptr<TextEntry>> _textEntries;
        std::unordered_map<std::string, std::unique_ptr<Button>> _buttons;

        std::unordered_map<std::string, std::unique_ptr<TextEntry>> _textUsernames;
        std::unordered_map<std::string, std::unique_ptr<Button>> _buttonsUsername;

        std::vector<std::string> _usernames;
};
