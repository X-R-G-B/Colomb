#pragma once

#include <string>

#define network (INetwork::GetInstance())

class INetwork {
    public:
        static INetwork &GetInstance();
        virtual bool init(const std::string &url, const unsigned int port) = 0;
        virtual void update() = 0;
        virtual bool send(const std::string &text) = 0;
        virtual bool hasPacket() = 0;
        virtual std::string receive() = 0;
};
