#pragma once

#include <memory>
#include <unordered_map>
#include "raylib-cpp.hpp"
#include "Button.hpp"
#include "GamesMode.hpp"
#include "IMenu.hpp"
#include "Participants.hpp"
#include "TextEntry.hpp"

class GamePendingMenu : public IMenu {
    public:
        GamePendingMenu(raylib::Window &window);

        void update(raylib::Window &window);
        void draw(raylib::Window &window);
        void free(raylib::Window &window);

    private:
        void onGameModeClicked(const std::string &gameMode);

        std::unordered_map<std::string, std::unique_ptr<TextEntry>> _textEntries;
        std::unordered_map<std::string, std::unique_ptr<Button>> _buttons;
        Participants _participants;
        GamesMode _gamesMode;
        bool _stateFirstSync = false;
        bool _isReady        = false;
};
