#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "raylib-cpp.hpp"
#include "Button.hpp"
#include "TextEntry.hpp"

class GamesMode {
    public:
        using OnGameModeClicked = std::function<void(const std::string &gameMode)>;

        GamesMode(raylib::Window &window);

        void update(raylib::Window &window);
        void draw(raylib::Window &window);
        void free(raylib::Window &window);

        void addGamesMode(const std::string &gamesMode);
        void clearGamesMode();

        void setCurrentGameMode(const std::string &gameMode);
        const std::string &getCurrentGameMode() const;

        void setOnGameModeClicked(OnGameModeClicked onGameModeClicked);

    private:
        std::unordered_map<std::string, std::unique_ptr<TextEntry>> _textEntries;
        std::unordered_map<std::string, std::unique_ptr<Button>> _buttons;

        std::unordered_map<std::string, std::unique_ptr<TextEntry>> _textGamesMode;

        std::vector<std::string> _gamesMode;
        std::string _currentGameMode;

        OnGameModeClicked _onGameModeClicked;
};
