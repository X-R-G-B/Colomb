#pragma once

#include <string>

#define globalValues (GlobalValues::GetInstance())

class GlobalValues {
    public:
        static GlobalValues &GetInstance();

        std::string _roomName = "";
        std::string _username = "";
};
