#pragma once

#include <string>
#include "boost/asio.hpp"
#include "INetwork.hpp"

class Network : public INetwork {
    public:
        Network(std::string url);

        bool send(std::string text) override;
        std::string receive() override;

    private:
        boost::asio::io_context _io_context;
        boost::asio::ip::tcp::socket _socket;
        std::string _incomming_text;
};
