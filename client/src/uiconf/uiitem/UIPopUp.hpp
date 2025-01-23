#pragma once

#include "IUI.hpp"

class UIPopUp : public IUI {
    public:
        UIPopUp();
        UIPopUp(const std::string &id, const nlohmann::json &config);

        bool modify(
            const std::string &identifier,
            const std::string &key,
            const nlohmann::json &value,
            const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets) override;
        void update(raylib::Window &window, float parentX = 0, float parentY = 0) override;
        void draw(raylib::Window &window, float parentX = 0, float parentY = 0) override;
        const std::string &getId() const override;
        bool getVisible() const;

    private:
        std::string _identifier;
        float _topLeftX;
        float _topLeftY;
        raylib::Color _bgColor;
        raylib::Color _fgColor;
        float _fgSize;
        std::string _text;
        std::map<std::string, std::string> _choices;
        bool _visible;
        raylib::Text _textR;
        std::vector<raylib::Text> _choicesR;
};
