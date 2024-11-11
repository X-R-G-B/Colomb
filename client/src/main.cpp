#include "raylib-cpp.hpp"
#include "ResourcesManager.hpp"

int main(int /*unused*/, const char **av)
{
#ifndef NDEBUG
    Logger::setLogLevel(LogLevel::Debug);
#else
    Logger::setLogLevel(LogLevel::Warn);
#endif
    ResourcesManager::init(av[0]);
    raylib::Window window(800, 600, "Colomb");

    window.SetTargetFPS(60);
    while (!window.ShouldClose()) {
        window.BeginDrawing();
        window.ClearBackground(raylib::Color::White());
        window.EndDrawing();
    }
    return 0;
}
