#pragma once

#include <string>
#include "nlohmann/json.hpp"

#define network (INetwork::GetInstance())

class INetwork {
    public:
        static INetwork &GetInstance();
        virtual bool init(const std::string &url, const unsigned int port) = 0;
        virtual void update()                                              = 0;
        virtual bool send(const nlohmann::json &data)                         = 0;
        virtual bool hasPacket()                                           = 0;
        virtual nlohmann::json receive()                                      = 0;
};
