#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "raylib-cpp.hpp"
#include "nlohmann/json.hpp"

class UIConf {
    class Asset {
        public:
            Asset(const std::string &identifier, const std::string &urlPath);

            std::string _identifier;
            std::string _urlPath;
    };

    class IUI {
        public:
            virtual ~IUI() = default;
            virtual void update(raylib::Window &window) = 0;
            virtual void draw(raylib::Window &window) = 0;
            virtual const std::string &getId() const = 0;
    };

    class UIDiv : public IUI {
        public:
            UIDiv();

            void update(raylib::Window &window) override;
            void draw(raylib::Window &window) override;
            const std::string &getId() const override;

            void addChildren(std::shared_ptr<IUI> children);

        private:
            std::string _identifier;
            float topLeftX;
            float topLeftY;
            std::vector<std::shared_ptr<IUI>> _childrens;
            std::unordered_map<std::string, int> _childrensIndexes;
    };

    class UIButtonImage : public IUI {
        public:
            UIButtonImage();

            void update(raylib::Window &window) override;
            void draw(raylib::Window &window) override;
            const std::string &getId() const override;

        private:
            std::string _identifier;
            float _topLeftX;
            float _topLeftY;
            std::shared_ptr<Asset> _image;
            bool _visible;
            bool _clickable;
    };

    class UIButtonText : public IUI {
        public:
            UIButtonText();

            void update(raylib::Window &window) override;
            void draw(raylib::Window &window) override;
            const std::string &getId() const override;

        private:
            std::string _identifier;
            float _topLeftX;
            float _topLeftY;
            raylib::Color _bgColor;
            raylib::Color _fgColor;
            std::string _text;
            bool _visible;
            bool _clickable;
    };

    class UITextEntry : public IUI {
        public:
            UITextEntry();

            void update(raylib::Window &window) override;
            void draw(raylib::Window &window) override;
            const std::string &getId() const override;

        private:
            std::string _identifier;
            float _topLeftX;
            float _topLeftY;
            raylib::Color _bgColor;
            raylib::Color _fgColor;
            std::string _placeholder;
    };

    class UIPopUp : public IUI {
        public:
            UIPopUp();

            void update(raylib::Window &window) override;
            void draw(raylib::Window &window) override;
            const std::string &getId() const override;

        private:
            std::string _identifier;
            float _topLeftX;
            float _topLeftY;
            raylib::Color _bgColor;
            raylib::Color _fgColor;
            std::string _text;
            std::map<std::string, std::string> _choices;
            bool _visible;
    };

    public:
        UIConf(const std::string &file);

    private:
        std::string _name;
        std::unordered_map<std::string, Asset> _assets;
        std::vector<std::shared_ptr<IUI>> _page;
        std::unordered_map<std::string, int> _pageIndexes;
        std::unordered_map<std::string, std::shared_ptr<UIPopUp>> _popups;
        nlohmann::json _json;
};
