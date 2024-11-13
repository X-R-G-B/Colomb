#define ENET_IMPLEMENTATION
#include "NetworkInternal.hpp"
#include "Logger.hpp"
#include "enet.h"

Network::Network()
{
}

Network::~Network()
{
    if (_initialized) {
        enet_deinitialize();
    }
    if (_client != nullptr) {
        enet_host_destroy(_client);
    }
}

bool Network::init(const std::string &url, const unsigned int port)
{
    _url = url;
    if (enet_initialize() != 0) {
        Logger::fatal("NETWORK: could not initialize the client");
        return false;
    }
    _client = enet_host_create(NULL, 1, 2, 0, 0);
    if (_client == nullptr) {
        Logger::fatal("NETWORK: could not create the client");
        return false;
    }
    ENetAddress address;
    enet_address_set_host(&address, _url.data());
    address.port = port;
    enet_host_connect(_client, &address, 2, 0);
    _initialized = true;
    return true;
}

void Network::update()
{
    ENetEvent event;
    while (enet_host_service(_client, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                Logger::debug("NETWORK: connection succesful");
                event.peer->data = nullptr;
                _server          = event.peer;
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                Logger::debug("NETWORK: receive data");
                _packets.push(std::string(reinterpret_cast<char *>(event.packet->data)));
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT: Logger::warn("NETWORK: connection closed"); break;
            default: Logger::error("NETWORK: unknow event type: " + std::to_string(event.type)); break;
        }
    }
}

bool Network::send(const std::string &text)
{
    ENetPacket *packet = enet_packet_create(text.data(), text.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    if (packet == nullptr) {
        return false;
    }
    enet_peer_send(_server, 0, packet);
    return true;
}

bool Network::hasPacket()
{
    return !_packets.empty();
}

std::string Network::receive()
{
    std::string packet = _packets.front();
    _packets.pop();
    return packet;
}
