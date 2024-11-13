#pragma once

#include <cstddef>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

#include "INetwork.hpp"
#include "enet.h"

class Network : public INetwork {
    public:
        class Peer : public INetwork::IPeer {
            public:
                Peer(const std::string &id, ENetPeer *peer);
                const std::string &getId() const override;
                ENetPeer *getPeer();

            private:
                std::string _id;
                ENetPeer *_peer;
        };
        class PeerHashFn {
            public:
                size_t operator()(const std::shared_ptr<Peer> &peer) const;
        };

        Network();
        ~Network();

        Network &operator=(const Network &other) = delete;
        Network &operator=(const Network other)  = delete;

        bool init(OnNewPeerFn onNewPeer, OnDisconnectPeerFn onDisconnectPeer) override;
        void update() override;
        bool send(std::shared_ptr<IPeer> peer, const std::string &text) override;
        bool hasPacket(std::shared_ptr<IPeer> peer) override;
        std::string receive(std::shared_ptr<IPeer> peer) override;

    private:
        bool _initialized = false;
        ENetHost *_server = nullptr;
        std::unordered_map<std::string, std::pair<std::shared_ptr<Peer>, std::queue<std::string>>> _packets;
        OnNewPeerFn _onNewPeer;
        OnDisconnectPeerFn _onDisconnectPeer;
};
