#include "GamesManager.hpp"
#include "Logger.hpp"
#include "ResourcesManager.hpp"
#include "FrameRateLimit.hpp"

int main(int /*unused*/, const char **av)
{
#ifndef NDEBUG
    Logger::setLogLevel(LogLevel::Debug);
#else
    Logger::setLogLevel(LogLevel::Warn);
#endif
    ResourcesManager::init(av[0]);
    FrameRateLimit<60> frl;

    gamesManager;
    while (true) {
        gamesManager.update();
        Logger::debug("test1");
        frl.sleep();
    }
    return 0;
}
