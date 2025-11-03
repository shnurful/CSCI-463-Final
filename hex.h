#pragma once

#include <cstdint>
#include <string>
#include <sstream>

class hex{
public:
    static std::string to_hex8(uint8_t i);
    static std::string to_hex32(uint32_t i);
    static std::string to_hex0x32(uint32_t i);
};
