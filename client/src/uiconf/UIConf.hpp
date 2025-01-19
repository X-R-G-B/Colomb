#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "raylib-cpp.hpp"
#include "Rectangle.hpp"
#include "nlohmann/json.hpp"

class UIConf {
        class Asset {
            public:
                Asset(const std::string &identifier, const std::string &urlPath);

                std::string _identifier;
                std::string _urlPath;
                std::string _path;
        };

        class IUI {
            public:
                virtual ~IUI() = default;
                virtual bool modify(
                    const std::string &identifier,
                    const std::string &key,
                    const nlohmann::json &value,
                    const std::unordered_map<std::string, std::shared_ptr<Asset>> &assets)        = 0;
                virtual void update(raylib::Window &window, float parentX = 0, float parentY = 0) = 0;
                virtual void draw(raylib::Window &window, float parentX = 0, float parentY = 0)   = 0;
                virtual const std::string &getId() const                                          = 0;
        };

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

        class UITextEntry : public IUI {
            public:
                UITextEntry();
                UITextEntry(const nlohmann::json &config);

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
                std::string _placeholder;
                bool _visible;
                bool _clickable;
                raylib::Text _textR;
                bool _wasClicked;
        };

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
