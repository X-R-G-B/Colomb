#pragma once

#include <string>

class INetwork {
    public:
        static INetwork &GetInstance(std::string url);
        virtual bool init() = 0;
        virtual void update() = 0;
        virtual bool send(std::string text) = 0;
        virtual bool hasPacket() = 0;
        virtual std::string receive() = 0;
};
