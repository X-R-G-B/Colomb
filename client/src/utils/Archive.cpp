#include "Archive.hpp"
#include <array>
#include <cstddef>
#include <string>
#include <vector>
#include "Logger.hpp"
#include "zstd.h"

std::vector<char> Archive::compress(const std::string &text)
{
    size_t compressedSize = ZSTD_compressBound(text.length());
    if (text.length() > compressedSize) {
        Logger::error("ARCHIVE: data is too large");
        return std::vector<char>();
    }
    std::vector<char> compressedData(compressedSize);
    const size_t result =
        ZSTD_compress(compressedData.data(), compressedSize, text.data(), text.length(), COMPRESSION_LEVEL);
    if (ZSTD_isError(result)) {
        Logger::error("ARCHIVE: when compressing data" + std::string(ZSTD_getErrorName(result)));
        return std::vector<char>();
    }
    compressedData.resize(result);
    return compressedData;
}

std::string Archive::decompress(const void *data, size_t size)
{
    if (size > MAX_PACKET_SIZE) {
        Logger::error("ARCHIVE: message too long");
        return std::string();
    }
    std::array<char, MAX_PACKET_SIZE> decompressedArray = {0};
    const size_t result = ZSTD_decompress(decompressedArray.data(), MAX_PACKET_SIZE, data, size);
    if (ZSTD_isError(result)) {
        Logger::error("ARCHIVE: when decompressing data" + std::string(ZSTD_getErrorName(result)));
        return std::string();
    }
    if (result > MAX_PACKET_SIZE) {
        Logger::error("ARCHIVE: decompressing size is too big");
        return std::string();
    }
    std::string text = "";
    for (const auto &v : decompressedArray) {
        text.push_back(v);
    }
    return text;
}
