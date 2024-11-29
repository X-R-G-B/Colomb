#pragma once

#include <cstddef>
#include <string>
#include <vector>

constexpr int MAX_PACKET_SIZE   = 2048;
constexpr int COMPRESSION_LEVEL = 1;

class Archive {
    public:
        static std::vector<char> compress(const std::string &text);
        static std::string decompress(const void *data, size_t size);
};
