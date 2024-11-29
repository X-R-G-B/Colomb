#include "Game.hpp"
#include <algorithm>
#include <memory>
#include "Catan.hpp"
#include "INetwork.hpp"
#include "Logger.hpp"

Game::Game()
{
    Logger::debug("GAME: created");
}

Game::~Game()
{
    Logger::debug("GAME: deleted");
}

void Game::update()
{
    if (_players.size() == 0) {
        _isFinished = true;
    }
    if (this->isStarted() && _game) {
        _game->update();
        return;
    }
    for (auto &[key, player] : _players) {
        while (network.hasPacket(player._peer)) {
            const auto message = network.receive(player._peer);
            if (!message.contains("type") || !message.at("type").is_string()) {
                continue;
            }
            const auto messageType = message.at("type").template get<std::string>();
            if (messageType == "ready") {
                if (!message.contains("ready") || !message.at("ready").is_boolean()) {
                    continue;
                }
                const auto readyState = message.at("ready").template get<bool>();
                player._ready         = readyState;
                for (const auto &[key_i, player_i] : _players) {
                    network.send(
                        player_i._peer,
                        {
                            {"type",   "ready"           },
                            {"player", player_i._username},
                            {"ready",  player_i._ready   },
                    });
                }
            } else if (messageType == "games") {
                // TODO: get a list of available games.
                network.send(
                    player._peer,
                    {
                        {"type",              "games"                  },
                        {"games_name",        _availableGameName       },
                        {"games_description", _availableGameDescription}
                });
            } else if (messageType == "select") {
                if (!message.contains("game") || !message.at("game").is_string()) {
                    continue;
                }
                if (player._peer->getId() != _keyOwner) {
                    Logger::error("GAME: peer is asking to select but is not owner");
                    network.send(
                        player._peer,
                        {
                            {"type",    "select"},
                            {"success", "false" },
                    });
                    continue;
                }
                const auto gameSelect = message.at("game").template get<std::string>();
                bool success = std::find(_availableGameName.begin(), _availableGameName.end(), gameSelect)
                    != _availableGameName.end();
                if (success) {
                    _selectedGame = gameSelect;
                    network.send(
                        player._peer,
                        {
                            {"type",     "select"  },
                            {"success",  true      },
                            {"gameName", gameSelect}
                    });
                    for (const auto &[key_i, player_i] : _players) {
                        if (key_i != key) {
                            network.send(
                                player_i._peer,
                                {
                                    {"type",     "select"  },
                                    {"gameName", gameSelect}
                            });
                        }
                    }
                } else {
                    Logger::error("GAME: peer is asking to select an unavailable game");
                    network.send(
                        player._peer,
                        {
                            {"type",    "select"},
                            {"success", false   }
                    });
                }
            } else if (messageType == "start") {
                if (this->startGame(player)) {
                    network.send(
                        player._peer,
                        {
                            {"type",     "start"      },
                            {"success",  true         },
                            {"gameName", _selectedGame}
                    });
                    for (const auto &[key_i, player_i] : _players) {
                        if (key_i != key) {
                            network.send(
                                player_i._peer,
                                {
                                    {"type",     "start"      },
                                    {"gameName", _selectedGame}
                            });
                        }
                    }
                } else {
                    network.send(
                        player._peer,
                        {
                            {"type",    "start"},
                            {"success", false  },
                    });
                }
            } else if (messageType == "state") {
                auto users       = std::vector<std::string>();
                auto users_ready = std::vector<bool>();
                for (const auto &[_, player_i] : _players) {
                    users.push_back(player_i._username);
                    users_ready.push_back(player_i._ready);
                }
                network.send(
                    player._peer,
                    {
                        {"type",          "state"                         },
                        {"players",       users                           },
                        {"players_ready", users_ready                     },
                        {"owner",         _players.at(_keyOwner)._username},
                        {"game",          _selectedGame                   }
                });
            } else {
                Logger::warn("GAME: unknow messageType");
            }
        }
    }
}

bool Game::isFinished()
{
    if (_isStarted && _game) {
        return _game->isFinished();
    }
    return _isFinished;
}

bool Game::isStarted()
{
    return _isStarted;
}

void Game::addPeer(std::shared_ptr<INetwork::IPeer> peer, const std::string &username)
{
    if (this->isPeerInside(peer)) {
        return;
    }
    _players[peer->getId()] = Player(peer, username);
    if (_players.size() == 1) {
        _keyOwner = peer->getId();
    }
    for (const auto &[key, p_i] : _players) {
        if (key != peer->getId()) {
            network.send(
                p_i._peer,
                {
                    {"type",   "newPlayer"                      },
                    {"player", _players[peer->getId()]._username}
            });
        }
    }
}

bool Game::isPeerInside(std::shared_ptr<INetwork::IPeer> peer)
{
    return _players.contains(peer->getId());
}

void Game::disconnectPeer(std::shared_ptr<INetwork::IPeer> peer)
{
    if (!this->isStarted()) {
        for (const auto &[key, p_i] : _players) {
            if (key != peer->getId()) {
                network.send(
                    p_i._peer,
                    {
                        {"type",   "delPlayer"                      },
                        {"player", _players[peer->getId()]._username}
                });
            }
        }
        _players.erase(peer->getId());
    } else {
        _players[peer->getId()]._isDisconnected = true;
    }
}

void Game::connectPeer(std::shared_ptr<INetwork::IPeer> peer)
{
    _players.at(peer->getId())._isDisconnected = false;
}

bool Game::startGame(const Player &player)
{
    if (_selectedGame.length() == 0) {
        return false;
    }
    if (std::find(_availableGameName.begin(), _availableGameName.end(), _selectedGame)
        == _availableGameName.end()) {
        return false;
    }
    if (player._peer->getId() != _keyOwner) {
        return false;
    }
    for (const auto &[_, player] : _players) {
        if (!player._ready) {
            return false;
        }
    }
    if (_selectedGame == "Catan") {
        _game = std::make_unique<Catan>();
        _game->init(_players);
    } else {
        Logger::error("GAME: unknow game name");
        return false;
    }
    _isStarted = true;
    return true;
}
