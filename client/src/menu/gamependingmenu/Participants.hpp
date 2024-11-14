#pragma once

#include <memory>
#include <unordered_map>
#include "raylib-cpp.hpp"
#include "Button.hpp"
#include "TextEntry.hpp"

class Participants {
    public:
        Participants(raylib::Window &window);

        void update(raylib::Window &window);
        void draw(raylib::Window &window);
        void free(raylib::Window &window);

    private:
        std::unordered_map<std::string, std::unique_ptr<TextEntry>> _textEntries;
        std::unordered_map<std::string, std::unique_ptr<Button>> _buttons;
};
