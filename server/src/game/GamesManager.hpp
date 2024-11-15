#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "Game.hpp"
#include "INetwork.hpp"

#define gamesManager (GamesManager::GetInstance())

class GamesManager {
    public:
        static GamesManager &GetInstance();
        GamesManager &operator=(const GamesManager &other) = delete;
        GamesManager &operator=(const GamesManager other)  = delete;

        void update();

    private:
        void onNewPeer(std::shared_ptr<INetwork::IPeer> peer);
        void onDisconnectPeer(std::shared_ptr<INetwork::IPeer> peer);
        GamesManager();
        std::string createGame();
        bool connectPeer(
            const std::string &roomName,
            std::shared_ptr<INetwork::IPeer> peer,
            const std::string &username);

        std::unordered_map<std::string, std::shared_ptr<INetwork::IPeer>> _globalLobby;
        std::unordered_map<std::string, std::shared_ptr<Game>> _games;

        std::vector<std::string> _toRemoveGlobalClient;
};
