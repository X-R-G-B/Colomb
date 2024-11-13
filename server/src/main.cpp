#include "GamesManager.hpp"
#include "ResourcesManager.hpp"

int main(int /*unused*/, const char **av)
{
#ifndef NDEBUG
    Logger::setLogLevel(LogLevel::Debug);
#else
    Logger::setLogLevel(LogLevel::Warn);
#endif
    ResourcesManager::init(av[0]);

    gamesManager;
    while (true) {
        gamesManager.update();
    }
    return 0;
}
