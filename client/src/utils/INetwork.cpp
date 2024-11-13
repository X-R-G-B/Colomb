#include "INetwork.hpp"
#include "NetworkInternal.hpp"

INetwork &INetwork::GetInstance()
{
    static Network networkInternal;
    return networkInternal;
}
