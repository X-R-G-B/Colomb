#pragma once

#include <string>
#include <unordered_map>
#include "Player.hpp"

class IGame {
    public:
        virtual ~IGame()                                                    = default;
        virtual void init(std::unordered_map<std::string, Player> &players) = 0;
        virtual void update()                                               = 0;
        virtual bool isFinished()                                           = 0;
};
