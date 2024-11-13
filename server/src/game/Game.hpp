#pragma once

#include <memory>
#include <unordered_map>
#include "INetwork.hpp"

class Game {
    public:
        class Player {
            public:
                Player(std::shared_ptr<INetwork::IPeer> peer);
                std::shared_ptr<INetwork::IPeer> _peer;
                bool _isDisconnected = false;
        };
        Game();

        void update();
        bool isFinished();
        bool isStarted();
        void addPeer(std::shared_ptr<INetwork::IPeer> peer);
        bool isPeerInside(std::shared_ptr<INetwork::IPeer> peer);
        void disconnectPeer(std::shared_ptr<INetwork::IPeer> peer);
        void connectPeer(std::shared_ptr<INetwork::IPeer> peer);

    private:
        bool _isFinished = false;
        bool _isStarted  = false;
        std::unordered_map<std::string, Player> _players;
};
