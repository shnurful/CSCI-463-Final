/* Ethan Silo
    z1838047
    CSCI 340-PE1

    I certify that this is my own work and where appropriate an extension
    of the starter code provided for the assignment.
*/
#pragma once

#include <cstdint>
#include <sstream>
#include <string>

/**
 * @class hex
 * @brief A utility class for formatting numbers into hexadecimal strings.
 *
 * This class provides methods to convert 8-bit and 32-bit unsigned
 * integers into various hexadecimal string representations.
 ********************************************************************************/
class hex {
public:
  /**
   * @brief Converts an 8-bit unsigned integer to a 2-character hex string.
   * @param i The uint8_t value to convert.
   * @return A 2-character std::string.
   ****************************************************************************/
  static std::string to_hex8(uint8_t i);

  /**
   * @brief Converts a 32-bit unsigned integer to an 8-character hex string.
   * @param i The uint32_t value to convert.
   * @return An 8-character std::string.
   ****************************************************************************/
  static std::string to_hex32(uint32_t i);

  /**
   * @brief Converts a 32-bit unsigned integer to an 8-character hex string
   * prepended with "0x".
   * @param i The uint32_t value to convert.
   * @return A 10-character std::string.
   ****************************************************************************/
  static std::string to_hex0x32(uint32_t i);

  static std::string to_hex0x20(uint32_t i); 

  static std::string to_hex0x12(uint32_t i);
};
