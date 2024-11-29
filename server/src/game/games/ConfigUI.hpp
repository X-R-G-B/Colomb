#pragma once

#include <string>
#include <unordered_map>
#include "INetwork.hpp"

class ConfigUI {
    public:
        ConfigUI(const std::string &identifier);

        void addPeer(std::shared_ptr<INetwork::IPeer> peer);
        bool isPeersOk();
        void update();

    private:
        class ConfigState {
            public:
                bool _askedHash   = false;
                bool _ok          = false;
                int _nbChunk      = -1;
                int _currentChunk = -1;
                std::string _hashClient;
        };

        std::string _identifier;
        std::string _fileHash;
        std::string _filePath;
        std::vector<std::string> _fileChunk;

        std::unordered_map<std::string, std::shared_ptr<INetwork::IPeer>> _peers;
        std::unordered_map<std::string, ConfigState> _peersState;
};
