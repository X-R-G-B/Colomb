#include "ResourcesManager.hpp"

int main(int /*unused*/, const char **av)
{
#ifndef NDEBUG
    Logger::setLogLevel(LogLevel::Debug);
#else
    Logger::setLogLevel(LogLevel::Warn);
#endif
    ResourcesManager::init(av[0]);

    while (true) {
        return 0;
    }
    return 0;
}
