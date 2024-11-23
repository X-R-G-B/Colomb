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
            virtual void update(raylib::Window &window, float parentX = 0, float parentY = 0) = 0;
            virtual void draw(raylib::Window &window, float parentX = 0, float parentY = 0) = 0;
            virtual const std::string &getId() const = 0;
    };

    class UIDiv : public IUI {
        public:
            UIDiv();

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

    class UIButtonImage : public IUI {
        public:
            UIButtonImage();

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

    class UIButtonText : public IUI {
        public:
            UIButtonText();

            void update(raylib::Window &window, float parentX = 0, float parentY = 0) override;
            void draw(raylib::Window &window, float parentX = 0, float parentY = 0) override;
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
            raylib::Text _textR;
    };

    class UITextEntry : public IUI {
        public:
            UITextEntry();

            void update(raylib::Window &window, float parentX = 0, float parentY = 0) override;
            void draw(raylib::Window &window, float parentX = 0, float parentY = 0) override;
            const std::string &getId() const override;

        private:
            std::string _identifier;
            float _topLeftX;
            float _topLeftY;
            raylib::Color _bgColor;
            raylib::Color _fgColor;
            std::string _placeholder;
            bool _visible;
            bool _clickable;
            raylib::Text _textR;
            bool _wasClicked;
    };

    class UIPopUp : public IUI {
        public:
            UIPopUp();

            void update(raylib::Window &window, float parentX = 0, float parentY = 0) override;
            void draw(raylib::Window &window, float parentX = 0, float parentY = 0) override;
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
            raylib::Text _textR;
            std::vector<raylib::Text> _choicesR;
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
