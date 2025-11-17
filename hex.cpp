/* Ethan Silo
    z1838047
    CSCI 340-PE1

    I certify that this is my own work and where appropriate an extension
    of the starter code provided for the assignment.
*/
/**
 * @file hex.cpp
 * @brief Implementation of the hex class methods.
 *
 * Provides the definitions for methods used to convert integer
 * values into formatted hexadecimal strings.
 ********************************************************************************/

#include "hex.h"
#include <iomanip>

/**
 * @brief Converts an 8-bit unsigned integer to a 2-character hex string.
 * @param i The uint8_t value to convert.
 * @return A 2-character std::string, zero-padded.
 ********************************************************************************/
std::string hex::to_hex8(uint8_t i) {
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(2)
     << static_cast<uint16_t>(i);
  return os.str();
}

/**
 * @brief Converts a 32-bit unsigned integer to an 8-character hex string.
 * @param i The uint32_t value to convert.
 * @return An 8-character std::string, zero-padded
 *********************************************************************************/
std::string hex::to_hex32(uint32_t i) {
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(8)
     << static_cast<uint32_t>(i);
  return os.str();
}

/**
 * @brief Converts a 32-bit unsigned integer to an 8-character hex string
 * prepended with "0x".
 * @param i The uint32_t value to convert.
 * @return A 10-character std::string.
 ********************************************************************************/
std::string hex::to_hex0x32(uint32_t i) {
  return std::string("0x") + hex::to_hex32(i);
}

std::string hex::to_hex0x20(uint32_t i) {
  std::ostringstream os;
  os << "0x" << std::hex << std::setfill('0') << std::setw(5) << i;
  return os.str();
}

std::string hex::to_hex0x12(uint32_t i) {
  std::ostringstream os;
  os << "0x" << std::hex << std::setfill('0') << std::setw(3) << i;
  return os.str();
}
