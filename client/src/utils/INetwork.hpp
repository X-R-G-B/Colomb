#pragma once

#include <string>

class INetwork {
    public:
        static INetwork &GetInstance(std::string url);
        virtual bool send(std::string text) = 0;
        virtual std::string receive() = 0;
};
