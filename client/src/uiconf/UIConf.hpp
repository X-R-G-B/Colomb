#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Asset.hpp"
#include "IUI.hpp"
#include "UIPopUp.hpp"

class UIConf {
    public:
        UIConf(const std::string &identifier);

        bool modify(const std::string &identifier, const std::string &key, const nlohmann::json &value);
        void update(raylib::Window &window);
        void draw(raylib::Window &window);

        static std::string toFile(const std::string &identifier);
        static std::string hash(const std::string &identifier);
        static bool appendString(const std::string &identifier, const std::string &data);
        static bool writeString(const std::string &identifier, const std::string &data);

    private:
        std::string _name;
        std::unordered_map<std::string, std::shared_ptr<Asset>> _assets;
        std::vector<std::shared_ptr<IUI>> _page;
        std::unordered_map<std::string, int> _pageIndexes;
        std::unordered_map<std::string, std::shared_ptr<UIPopUp>> _popups;
        nlohmann::json _json;
};
