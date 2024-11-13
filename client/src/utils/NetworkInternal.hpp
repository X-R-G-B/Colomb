#pragma once

#include <string>
#include <queue>

#include "enet.h"
#include "INetwork.hpp"

class Network : public INetwork {
    public:
        Network();
        ~Network();

        Network &operator=(const Network &other) = delete;
        Network &operator=(const Network other) = delete;

        bool init(const std::string &url, const unsigned int port) override;
        void update() override;
        bool send(const std::string &text) override;
        bool hasPacket() override;
        std::string receive() override;

    private:
        bool _initialized = false;
        std::string _url;
        ENetHost *_client = nullptr;
        ENetPeer *_server = nullptr;
        std::queue<std::string> _packets;
};
