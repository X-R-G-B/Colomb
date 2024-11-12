#include "Logger.hpp"
#include "NetworkInternal.hpp"

Network::Network(std::string url): _io_context(), _socket(_io_context)
{
    boost::asio::ip::tcp::resolver resolver(_io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(url, "Colomb");

    boost::asio::connect(_socket, endpoints);

    boost::asio::post(_io_context, [this]()
    {
        
    });
}

bool Network::send(std::string text)
{
    _socket.async_send(boost::asio::buffer(text),
        [text](const boost::system::error_code &error, std::size_t bytes_sent)
    {
        Logger::debug("NETWORK: Package sent");
        if (error) {
            Logger::error("NETWORK: " + std::string(error.message()));
            return;
        }
        if (bytes_sent != text.size()) {
            Logger::error("NETWORK: Package not sent");
            return;
        }
    });
    return true;
}

std::string receive()
{
    
}
