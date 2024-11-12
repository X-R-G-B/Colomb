#include "NetworkInternal.hpp"
#include "INetwork.hpp"

INetwork &INetwork::GetInstance(std::string url)
{
    static Network network(url);
    return network;
}
