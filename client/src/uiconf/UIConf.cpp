#include "UIConf.hpp"

UIConf::UIConf(const std::string &file):
    _json(nlohmann::json::parse(file))
{
}
