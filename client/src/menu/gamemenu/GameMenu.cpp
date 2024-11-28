#include "GameMenu.hpp"
#include <memory>
#include "INetwork.hpp"
#include "UIConf.hpp"

GameMenu::GameMenu(raylib::Window &window)
{
}

void GameMenu::update(raylib::Window &window)
{
    while (network.hasPacket()) {
        const auto message = network.receive();
        if (!message.contains("type") || !message.at("type").is_string()) {
            continue;
        }
        const auto messageType = message.at("type").template get<std::string>();
        if (messageType == "uiConfigHash") {
            if (!message.contains("name") || !message.at("name").is_string()) {
                continue;
            }
            const auto name = message.at("name").template get<std::string>();
            network.send({
                {"type", "uiConfigHash"},
                {"name", name},
                {"hash", UIConf::hash(name)},
            });
        } else if (messageType == "uiConfig") {
            if (!message.contains("name") || !message.contains("nbChunk") || !message.contains("chunkIndex") || !message.contains("data") || !message.at("name").is_string() || !message.at("nbChunk").is_number() || !message.at("chunkIndex").is_number() || !message.at("data").is_string()) {
                continue;
            }
            const auto name = message.at("name").template get<std::string>();
            const auto nbChunk = message.at("nbChunk").template get<int>();
            const auto chunkIndex = message.at("chunkIndex").template get<int>();
            const auto data = message.at("data").template get<std::string>();
            if (chunkIndex == 0) {
                UIConf::writeString(name, data);
            } else {
                UIConf::appendString(name, data);
            }
            if (chunkIndex == nbChunk - 1) {
                network.send({
                    {"type", "uiConfig"},
                    {"name", name},
                    {"nbChunk", nbChunk},
                });
            }
        } else if (messageType == "change") {
            if (!message.contains("id") || !message.contains("key") || !message.contains("value") || !message.at("id").is_string() || !message.at("key").is_string()) {
                continue;
            }
            const auto id = message.at("id").template get<std::string>();
            const auto key = message.at("key").template get<std::string>();
            const auto value = message.at("value").template get<std::string>();
            if (_uiConf) {
                _uiConf->modify(id, key, value);
            }
        } else if (messageType == "setConfig") {
            if (!message.contains("name") || !message.at("name").is_string()) {
                continue;
            }
            const auto name = message.at("name").template get<std::string>();
            _uiConf = std::make_shared<UIConf>(UIConf::toFile(name));
        }
    }
}

void GameMenu::draw(raylib::Window &window)
{
}

void GameMenu::free(raylib::Window &window)
{
}
