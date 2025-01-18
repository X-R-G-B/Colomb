#include "ConfigUI.hpp"
#include <fstream>
#include <functional>
#include <string>
#include "INetwork.hpp"
#include "Logger.hpp"
#include "PathResolver.hpp"


std::vector<std::string> split(const std::string& str, int splitLength)
{
    int NumSubstrings = str.length() / splitLength;
    std::vector<std::string> ret;

    for (auto i = 0; i < NumSubstrings; i++)
    {
        ret.push_back(str.substr(i * splitLength, splitLength));
    }

    // If there are leftover characters, create a shorter item at the end.
    if (str.length() % splitLength != 0)
    {
        ret.push_back(str.substr(splitLength * NumSubstrings));
    }

    return ret;
}

ConfigUI::ConfigUI(const std::string &identifier)
    : _identifier(identifier),
      _filePath(PathResolver::resolve("assets/uiconf/" + identifier))
{
    std::ifstream file(_filePath);
    std::string all;

    file.seekg(0, std::ios::end);
    all.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    all.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
    _fileChunk = split(all, 500);
    _fileHash = std::to_string(std::hash<std::string> {}(all));
    Logger::debug("hash_" + _identifier + "_" + _fileHash);
}

void ConfigUI::addPeer(std::shared_ptr<INetwork::IPeer> peer)
{
    _peers[peer->getId()]      = peer;
    _peersState[peer->getId()] = ConfigState();
}

void ConfigUI::update()
{
    for (auto &[key, pState] : _peersState) {
        if (pState._ok) {
            continue;
        }
        const auto &pEer = _peers[key];
        while (network.hasPacket(pEer)) {
            const auto message = network.receive(pEer);
            if (!message.contains("type") || !message.at("type").is_string()) {
                continue;
            }
            const auto messageType = message.at("type").template get<std::string>();
            if (messageType == "uiConfigHash") {
                if (!message.contains("name") || !message.contains("hash")
                    || !message.at("name").is_string() || !message.at("hash").is_string()) {
                    continue;
                }
                const auto name = message.at("name").template get<std::string>();
                const auto hash = message.at("hash").template get<std::string>();
                if (name != _identifier) {
                    network.send(
                        pEer,
                        {
                            {"type", "uiConfigHash"},
                            {"name", _identifier   },
                    });
                    continue;
                }
                if (hash != _fileHash) {
                    pState._ok           = false;
                    pState._nbChunk      = _fileChunk.size();
                    pState._currentChunk = -1;
                    pState._hashClient   = "";
                } else {
                    pState._ok = true;
                    Logger::debug("CONFIGUI: one client ok");
                }
            } else if (messageType == "uiConfig") {
                if (!message.contains("name") || !message.contains("nbChunk")
                    || !message.at("name").is_string() || !message.at("nbChunk").is_number()) {
                    continue;
                }
                network.send(
                    pEer,
                    {
                        {"type", "uiConfigHash"},
                        {"name", _identifier   },
                });
            }
        }
        if (pState._ok) {
            continue;
        }
        if (!pState._askedHash) {
            network.send(
                _peers[key],
                {
                    {"type", "uiConfigHash"},
                    {"name", _identifier   },
            });
            pState._askedHash = true;
            continue;
        }
        if (pState._nbChunk == -1) {
            continue;
        }
        if (pState._currentChunk == pState._nbChunk) {
            continue;
        }
        pState._currentChunk += 1;
        if (pState._currentChunk == pState._nbChunk) {
            continue;
        }
        network.send(
            pEer,
            {
                {"type",       "uiConfig"                      },
                {"name",       _identifier                     },
                {"nbChunk",    pState._nbChunk                 },
                {"chunkIndex", pState._currentChunk            },
                {"data",       _fileChunk[pState._currentChunk]},
        });
    }
    return;
}

bool ConfigUI::isPeersOk()
{
    for (const auto &[_, pState] : _peersState) {
        if (!pState._ok) {
            return false;
        }
    }
    return true;
}
