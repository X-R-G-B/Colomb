#include "Game.hpp"
#include <memory>
#include "INetwork.hpp"

Game::Player::Player(std::shared_ptr<INetwork::IPeer> peer): _peer(peer)
{
}

Game::Game()
{
}

void Game::update()
{
}

bool Game::isFinished()
{
    return _isFinished;
}

bool Game::isStarted()
{
    return _isStarted;
}

void Game::addPeer(std::shared_ptr<INetwork::IPeer> peer)
{
    if (this->isPeerInside(peer)) {
        return;
    }
    _players.at(peer->getId()) = Player(peer);
}

bool Game::isPeerInside(std::shared_ptr<INetwork::IPeer> peer)
{
    return _players.contains(peer->getId());
}

void Game::disconnectPeer(std::shared_ptr<INetwork::IPeer> peer)
{
    _players.at(peer->getId())._isDisconnected = true;
}

void Game::connectPeer(std::shared_ptr<INetwork::IPeer> peer)
{
    _players.at(peer->getId())._isDisconnected = false;
}
