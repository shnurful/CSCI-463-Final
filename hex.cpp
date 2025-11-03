#include "hex.h"
#include <iomanip>

std::string to_hex8(uint8_t i) {
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(2)
     << static_cast<uint16_t>(i);
  return os.str();
}

std::string to_hex32(uint32_t i) {
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(2)
     << static_cast<uint32_t>(i);
  return os.str();
}

std::string to_hex0x32(uint32_t i) { return std::string("0x") + to_hex0x32(i); }
