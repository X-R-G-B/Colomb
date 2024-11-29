#pragma once

#include <memory>
#include "INetwork.hpp"

class Player {
    public:
        Player(std::shared_ptr<INetwork::IPeer> peer, const std::string &username);
        Player() = default;
        std::shared_ptr<INetwork::IPeer> _peer;
        bool _isDisconnected = false;
        bool _ready          = false;
        std::string _username;
};
