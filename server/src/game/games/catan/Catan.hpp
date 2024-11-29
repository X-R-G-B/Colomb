#pragma once

#include "IGame.hpp"
#include "ConfigUI.hpp"

class Catan : public IGame {
    public:
        Catan();
        void init(std::unordered_map<std::string, Player> &players) override;
        void update() override;
        bool isFinished() override;

    private:
        class PlayerState {
            public:
        };

        std::unordered_map<std::string, Player> _tmpPlayers;
        std::unordered_map<std::string, Player> &_players;

        std::unordered_map<std::string, PlayerState> _playersState;

        bool _isFinished = false;
        bool _allConfigOk = false;

        std::unique_ptr<ConfigUI> _configUi;
};
