#include "Catan.hpp"
#include <memory>
#include "Logger.hpp"

Catan::Catan() : _players(_tmpPlayers)
{
    Logger::debug("Catan game created");
}

void Catan::init(std::unordered_map<std::string, Player> &players)
{
    _players  = players;
    _configUi = std::make_unique<ConfigUI>("catan.json");
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
        return;
    }
}

bool Catan::isFinished()
{
    return _isFinished;
}
