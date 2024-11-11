#include "raylib-cpp.hpp"
#include "ResourcesManager.hpp"

#include "FirstMenu.hpp"

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

    auto firstmenu = FirstMenu(window);
    while (!window.ShouldClose()) {
        firstmenu.update(window);
        window.BeginDrawing();
        window.ClearBackground(raylib::Color::White());
        firstmenu.draw(window);
        window.EndDrawing();
    }
    firstmenu.free(window);
    return 0;
}
