#include "NetworkInternal.hpp"
#include "INetwork.hpp"

INetwork &INetwork::GetInstance()
{
    static Network networkInternal;
    return networkInternal;
}
