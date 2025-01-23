#pragma once

#include "IUI.hpp"

class UIButtonText : public IUI {
    public:
        UIButtonText();
        UIButtonText(const nlohmann::json &config);

        bool modify(
            const std::string &identifier,
            const std::string &key,
            const nlohmann::json &value,
            const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets) override;
        void update(raylib::Window &window, float parentX = 0, float parentY = 0) override;
        void draw(raylib::Window &window, float parentX = 0, float parentY = 0) override;
        const std::string &getId() const override;

    private:
        std::string _identifier;
        float _topLeftX;
        float _topLeftY;
        raylib::Color _bgColor;
        raylib::Color _fgColor;
        float _fgSize;
        float _fgSpacing = 2;
        std::string _text;
        bool _visible;
        bool _clickable;
        raylib::Text _textR;
        raylib::Rectangle _rectR;
};
