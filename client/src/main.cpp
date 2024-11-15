#include "raylib-cpp.hpp"
#include "INetwork.hpp"
#include "MenusManager.hpp"
#include "ResourcesManager.hpp"
#include "PathResolver.hpp"

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
    auto iconImage = raylib::Image(PathResolver::resolve("assets/icons/favicon.png"));
    window.SetIcon(iconImage);

    network.init(SERVER_URL, SERVER_PORT);
    auto menus = MenusManager(window);
    while (!window.ShouldClose()) {
        network.update();
        menus.update(window);
        window.BeginDrawing();
        window.ClearBackground(raylib::Color::DarkBlue());
        menus.draw(window);
#ifndef NDEBUG
        window.DrawFPS();
#endif
        window.EndDrawing();
    }
    menus.free(window);
    return 0;
}
