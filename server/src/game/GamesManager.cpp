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
            if (!message.contains("type") || !message.at("type").is_string()) {
                continue;
            }
            const auto messageType = message.at("type").template get<std::string>();
            if (messageType == "join") {
                if (!message.contains("roomName") || !message.contains("username")
                    || !message.at("roomName").is_string() || !message.at("username").is_string()) {
                    continue;
                }
                const auto roomName = message.at("roomName").template get<std::string>();
                const auto username = message.at("username").template get<std::string>();
                if (this->connectPeer(roomName, peer, username)) {
                    network.send(
                        peer,
                        {
                            {"type",     "join"  },
                            {"success",  true    },
                            {"username", username},
                            {"roomName", roomName}
                    });
                } else {
                    network.send(
                        peer,
                        {
                            {"type",    "join"},
                            {"success", false },
                    });
                }
            } else if (messageType == "create") {
                if (!message.contains("username") || !message.at("username").is_string()) {
                    continue;
                }
                const auto username = message.at("username").template get<std::string>();
                const auto roomName = this->createGame();
                if (this->connectPeer(roomName, peer, username)) {
                    network.send(
                        peer,
                        {
                            {"type",     "create"},
                            {"success",  true    },
                            {"username", username},
                            {"roomName", roomName},
                    });
                } else {
                    network.send(
                        peer,
                        {
                            {"type",    "create"},
                            {"success", false   },
                    });
                }
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
    for (const auto &key : _toRemoveGlobalClient) {
        _globalLobby.erase(key);
    }
    _toRemoveGlobalClient.clear();
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

bool GamesManager::connectPeer(
    const std::string &roomName,
    std::shared_ptr<INetwork::IPeer> peer,
    const std::string &username)
{
    std::string usernameModif(username);

    if (!_globalLobby.contains(peer->getId())) {
        return false;
    }
    if (!_games.contains(roomName)) {
        return false;
    }
    if (usernameModif.length() > 15) {
        usernameModif.resize(15);
    }
    _toRemoveGlobalClient.push_back(peer->getId());
    _games[roomName]->addPeer(peer, usernameModif);
    return true;
}
