#include "Player.hpp"

Player::Player(std::shared_ptr<INetwork::IPeer> peer, const std::string &username)
    : _peer(peer),
      _username(username)
{
}
