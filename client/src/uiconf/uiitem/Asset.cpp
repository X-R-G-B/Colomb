#include "httplib.h"
#include "Logger.hpp"
#include "Asset.hpp"
#include "PathResolver.hpp"

Asset::Asset(const std::string &identifier, const std::string &urlPath)
    : _identifier(identifier),
      _urlPath(urlPath)
{
    std::string host(_urlPath);
    host.erase(host.find('/', host.find("://") + 3));
    std::string path(_urlPath.substr(host.size()));

    Logger::info("UICONF_ASSET: Downloading host=" + host + "; path=" + path + ";");
    httplib::Client cli(host);

    cli.set_follow_location(true);
    const auto &res = cli.Get(path);
    if (res->status != 200) {
        throw std::runtime_error("This should not happend: failed to load assets");
    }
    const auto filePath = PathResolver::resolve(identifier);
    std::ofstream file(filePath);
    file << res->body;
    file.close();
    _path = filePath;
}
