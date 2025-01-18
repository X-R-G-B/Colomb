#include <memory>
#include <string>
#include "Catan.hpp"
#include "INetwork.hpp"
#include "Logger.hpp"

static const std::string game_identifier = "catan.json";

Catan::Catan() : _players(_tmpPlayers)
{
    Logger::debug("Catan game created");
}

void Catan::init(std::unordered_map<std::string, Player> &players)
{
    _players  = players;
    _configUi = std::make_unique<ConfigUI>(game_identifier);
    for (const auto &[key, pLayer] : _players) {
        _playersState[key] = PlayerState();
        _configUi->addPeer(pLayer._peer);
    }
}

void Catan::update()
{
    if (!_allConfigOk && _configUi) {
        _configUi->update();
        _allConfigOk = _configUi->isPeersOk();
        if (_allConfigOk) {
            for (const auto &[_, pLayer] : _players) {
                network.send(pLayer._peer, {
                    {"type", "setConfig"},
                    {"name", game_identifier}
                });
            }
        }
        return;
    }
}

bool Catan::isFinished()
{
    return _isFinished;
}
