#pragma once

#include <string>

class Asset {
    public:
        Asset(const std::string &identifier, const std::string &urlPath);

        std::string _identifier;
        std::string _urlPath;
        std::string _path;
};
