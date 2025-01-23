#pragma once

#include "IUI.hpp"

class UIDiv : public IUI {
    public:
        UIDiv();
        UIDiv(
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

        void addChildren(std::shared_ptr<IUI> children);

    private:
        std::string _identifier;
        float _topLeftX;
        float _topLeftY;
        std::vector<std::shared_ptr<IUI>> _childrens;
        std::unordered_map<std::string, int> _childrensIndexes;
};

namespace UIDivConverter {
    void from_json(const nlohmann::json &j, UIDiv &div);
}
