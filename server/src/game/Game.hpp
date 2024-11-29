#pragma once

#include <memory>
#include "IGame.hpp"
#include "INetwork.hpp"
#include "Player.hpp"

class Game {
    public:
        Game();
        ~Game();

        void update();
        bool isFinished();
        bool isStarted();
        void addPeer(std::shared_ptr<INetwork::IPeer> peer, const std::string &username);
        bool isPeerInside(std::shared_ptr<INetwork::IPeer> peer);
        void disconnectPeer(std::shared_ptr<INetwork::IPeer> peer);
        void connectPeer(std::shared_ptr<INetwork::IPeer> peer);

    private:
        bool startGame(const Player &player);

        bool _isFinished          = false;
        bool _isStarted           = false;
        std::string _selectedGame = "";
        std::unordered_map<std::string, Player> _players;
        std::string _keyOwner;

        std::vector<std::string> _availableGameName        = {"Catan", "Test"};
        std::vector<std::string> _availableGameDescription = {"The catan game", "test game"};
        std::unique_ptr<IGame> _game;
};
