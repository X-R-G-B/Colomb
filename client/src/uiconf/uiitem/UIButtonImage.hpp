#pragma once

#include "IUI.hpp"

class UIButtonImage : public IUI {
    public:
        UIButtonImage();
        UIButtonImage(
            const nlohmann::json &config,
            const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets);

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
        std::shared_ptr<Asset> _image;
        raylib::Texture _texture;
        bool _visible;
        bool _clickable;
};
