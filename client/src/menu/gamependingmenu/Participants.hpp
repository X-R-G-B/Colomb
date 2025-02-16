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

        void addParticipant(raylib::Window &window, const std::string &username);
        void clearParticipants();
        void removeParticipant(const std::string &username);

        void setOwner(const std::string &username);
        const std::string &getOwner() const;
        void setParticipantReady(const std::string &username, bool ready);

    private:
        std::unordered_map<std::string, std::unique_ptr<TextEntry>> _textEntries;
        std::unordered_map<std::string, std::unique_ptr<Button>> _buttons;

        std::unordered_map<std::string, std::unique_ptr<TextEntry>> _textUsernames;
        std::unordered_map<std::string, std::unique_ptr<Button>> _buttonsUsername;

        std::vector<std::string> _usernames;
        std::string _owner;
        bool _ownerSet = false;
};
