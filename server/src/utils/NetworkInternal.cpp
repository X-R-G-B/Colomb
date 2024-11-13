#include <memory>
#include "Archive.hpp"
#define ENET_IMPLEMENTATION
#include "Logger.hpp"
#include "NetworkInternal.hpp"
#include "enet.h"

Network::Network()
{
}

Network::~Network()
{
    if (_initialized) {
        enet_deinitialize();
    }
    if (_server != nullptr) {
        enet_host_destroy(_server);
    }
}

bool Network::init(OnNewPeerFn onNewPeer, OnDisconnectPeerFn onDisconnectPeer)
{
    _onNewPeer        = onNewPeer;
    _onDisconnectPeer = onDisconnectPeer;
    if (enet_initialize() != 0) {
        Logger::fatal("NETWORK: could not initialize the server");
        return false;
    }
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = SERVER_PORT;
    _server      = enet_host_create(&address, 32, 2, 0, 0);
    if (_server == nullptr) {
        Logger::fatal("NETWORK: could not create the server");
        return false;
    }
    _initialized = true;
    return true;
}

static std::string enet_peer_to_id(ENetPeer *peer)
{
    char ip_address[64] = {0};

    enet_address_get_host_ip(&peer->address, ip_address, 63);
    return std::string(ip_address) + std::string(":") + std::to_string(peer->address.port);
}

void Network::update()
{
    ENetEvent event;
    while (enet_host_service(_server, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                Logger::debug("NETWORK: client connected");
                {
                    auto id         = enet_peer_to_id(event.peer);
                    auto peer       = std::make_shared<Peer>(id, event.peer);
                    _packets.at(id) = std::make_pair(peer, std::queue<std::string>());
                    _onNewPeer(peer);
                }
                event.peer->data = nullptr;
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                Logger::debug("NETWORK: receive data");
                {
                    auto id      = enet_peer_to_id(event.peer);
                    auto message = Archive::decompress(event.packet->data, event.packet->dataLength);
                    _packets.at(id).second.push(message);
                }
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                Logger::warn("NETWORK: client disconnected");
                {
                    auto id = enet_peer_to_id(event.peer);
                    _onDisconnectPeer(_packets.at(id).first);
                    _packets.erase(id);
                }
                break;
            default: Logger::error("NETWORK: unknow event type: " + std::to_string(event.type)); break;
        }
    }
}

bool Network::send(std::shared_ptr<IPeer> peer, const nlohmann::json &data)
{
    auto peer_                = std::static_pointer_cast<Peer>(peer);
    const auto text           = data.dump();
    const auto textCompressed = Archive::compress(text);
    ENetPacket *packet =
        enet_packet_create(textCompressed.data(), textCompressed.size(), ENET_PACKET_FLAG_RELIABLE);
    if (packet == nullptr) {
        return false;
    }
    enet_peer_send(peer_->getPeer(), 0, packet);
    return true;
}

bool Network::hasPacket(std::shared_ptr<IPeer> peer)
{
    auto peer_ = std::static_pointer_cast<Peer>(peer);
    if (_packets.contains(peer_->getId())) {
        return !_packets.at(peer_->getId()).second.empty();
    }
    return false;
}

nlohmann::json Network::receive(std::shared_ptr<IPeer> peer)
{
    auto peer_         = std::static_pointer_cast<Peer>(peer);
    std::string packet = _packets.at(peer_->getId()).second.front();
    _packets.at(peer_->getId()).second.pop();
    return nlohmann::json(packet);
}

Network::Peer::Peer(const std::string &id, ENetPeer *peer) : _id(id), _peer(peer)
{
}

const std::string &Network::Peer::getId() const
{
    return _id;
}

ENetPeer *Network::Peer::getPeer()
{
    return _peer;
}

size_t Network::PeerHashFn::operator()(const std::shared_ptr<Peer> &peer) const
{
    const auto hash = std::hash<std::string> {};
    return hash(peer->getId());
}
