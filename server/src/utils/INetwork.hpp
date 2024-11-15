#pragma once

#include <functional>
#include <memory>
#include <string>
#include "nlohmann/json.hpp"

#define network (INetwork::GetInstance())

class INetwork {
    public:
        class IPeer {
            public:
                virtual const std::string &getId() const = 0;
        };
        using OnNewPeerFn        = std::function<void(std::shared_ptr<IPeer>)>;
        using OnDisconnectPeerFn = std::function<void(std::shared_ptr<IPeer>)>;
        virtual ~INetwork()      = default;
        static INetwork &GetInstance();
        virtual bool init(OnNewPeerFn onNewPeer, OnDisconnectPeerFn onDisconnectPeer) = 0;
        virtual void update()                                                         = 0;
        virtual bool send(std::shared_ptr<IPeer> peer, const nlohmann::json &data)    = 0;
        virtual bool hasPacket(std::shared_ptr<IPeer> peer)                           = 0;
        virtual nlohmann::json receive(std::shared_ptr<IPeer> peer)                   = 0;
};
