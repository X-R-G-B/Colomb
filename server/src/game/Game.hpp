#pragma once

#include <memory>
#include <unordered_map>
#include "INetwork.hpp"

class Game {
    public:
        class Player {
            public:
                Player(std::shared_ptr<INetwork::IPeer> peer, const std::string &username);
                Player() = default;
                std::shared_ptr<INetwork::IPeer> _peer;
                bool _isDisconnected = false;
                bool _ready          = false;
                std::string _username;
        };
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
};
