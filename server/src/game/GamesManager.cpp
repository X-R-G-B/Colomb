#include "GamesManager.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "INetwork.hpp"
#include "Logger.hpp"
#include "nlohmann/json.hpp"

GamesManager::GamesManager()
{
    std::srand(std::time(nullptr));
    network.init(
        [this](std::shared_ptr<INetwork::IPeer> peer) {
            this->onNewPeer(peer);
        },
        [this](std::shared_ptr<INetwork::IPeer> peer) {
            this->onDisconnectPeer(peer);
        });
}

GamesManager &GamesManager::GetInstance()
{
    static GamesManager gameManager;
    return gameManager;
}

void GamesManager::onNewPeer(std::shared_ptr<INetwork::IPeer> peer)
{
    _globalLobby[peer->getId()] = peer;
}

void GamesManager::onDisconnectPeer(std::shared_ptr<INetwork::IPeer> peer)
{
    for (auto &[key, game] : _games) {
        if (game->isPeerInside(peer)) {
            game->disconnectPeer(peer);
            break;
        }
    }
}

void GamesManager::update()
{
    std::vector<std::string> removeGame;

    network.update();
    for (auto &[key, peer] : _globalLobby) {
        while (network.hasPacket(peer)) {
            const auto message = network.receive(peer);
            if (message["type"] == "join") {
                if (this->connectPeer(message["roomName"], peer)) {
                    nlohmann::json r = {
                        {"type",    "join"},
                        {"success", true  },
                    };
                    network.send(peer, r);
                } else {
                    nlohmann::json r = {
                        {"type",    "join"},
                        {"success", false },
                    };
                    network.send(peer, r);
                }
            } else if (message["type"] == "create") {
                auto roomName    = this->createGame();
                nlohmann::json r = {
                    {"type",     "create"},
                    {"roomName", roomName},
                };
                network.send(peer, r);
            } else {
                Logger::warn("GAMESMANAGER: unknow messageType");
            }
        }
    }
    for (auto &[key, game] : _games) {
        game->update();
        if (game->isFinished()) {
            removeGame.push_back(key);
        }
    }
    for (const auto &key : removeGame) {
        _games.erase(key);
    }
}

static std::string generateRandomRoomName()
{
    unsigned int number = 0;
    for (int i = 0; i < 5; i++) {
        number += std::rand() % 10 * (std::pow(10, i));
    }
    return std::to_string(number);
}

std::string GamesManager::createGame()
{
    auto newGameName = generateRandomRoomName();
    while (_games.contains(newGameName)) {
        newGameName = generateRandomRoomName();
    }
    _games[newGameName] = std::make_shared<Game>();
    return newGameName;
}

bool GamesManager::connectPeer(const std::string &roomName, std::shared_ptr<INetwork::IPeer> peer)
{
    if (!_globalLobby.contains(peer->getId())) {
        return false;
    }
    if (!_games.contains(roomName)) {
        return false;
    }
    _globalLobby.erase(peer->getId());
    _games[roomName]->addPeer(peer);
    return true;
}
