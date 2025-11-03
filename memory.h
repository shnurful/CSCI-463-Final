/* Ethan Silo
    z1838047
    CSCI 340-PE1
    
    I certify that this is my own work and where appropriate an extension 
    of the starter code provided for the assignment.
*/
#pragma once

#include "hex.h"
#include <cstdint>
#include <string>
#include <vector>

/**
 * @class memory
 * @brief Simulates a chunk of computer memory.
 *
 * Manages a vector of bytes, providing methods to read and write 8, 16,
 * and 32-bit values in a little-endian format. It also handles memory
 * bounds-checking and can load a file into its simulated memory.
 * It inherits from 'hex' to get access to the formatting utilities.
 ********************************************************************************/
class memory : public hex {
public:
  /**
   * @brief Constructs a new memory object.
   * @param s The desired size of the memory. Will be rounded up to the
   * nearest multiple of 16.
   ****************************************************************************/
  memory(uint32_t s);

  /**
   * @brief Destroys the memory object and clears the internal vector.
   ****************************************************************************/
  ~memory();

  /**
   * @brief Checks if a given address is outside the allocated memory size.
   * @param addr The address to check.
   * @return true if the address is out of bounds, false otherwise.
   * @note Also prints a warning to std::cout if out of bounds.
   ****************************************************************************/
  bool check_illegal(uint32_t addr) const;

  /**
   * @brief Gets the total allocated size of the memory.
   * @return The size of the memory in bytes (always a multiple of 16).
   ****************************************************************************/
  uint32_t get_size() const;

  /**
   * @brief Reads an 8-bit unsigned value from memory.
   * @param addr The address to read from.
   * @return The uint8_t value at the address, or 0 if the address
   * is illegal.
   ****************************************************************************/
  uint8_t get8(uint32_t addr) const;

  /**
   * @brief Reads a 16-bit unsigned value from memory (little-endian).
   * @param addr The starting address to read from.
   * @return The uint16_t value, or 0 if any part of the address
   * is illegal.
   ****************************************************************************/
  uint16_t get16(uint32_t addr) const;

  /**
   * @brief Reads a 32-bit unsigned value from memory (little-endian).
   * @param addr The starting address to read from.
   * @return The uint32_t value, or 0 if any part of the address
   * is illegal.
   ****************************************************************************/
  uint32_t get32(uint32_t addr) const;

  /**
   * @brief Reads an 8-bit value and sign-extends it to 32 bits.
   * @param addr The address to read from.
   * @return The int32_t sign-extended value.
   ****************************************************************************/
  int32_t get8_sx(uint32_t addr) const;

  /**
   * @brief Reads a 16-bit value and sign-extends it to 32 bits.
   * @param addr The starting address to read from.
   * @return The int32_t sign-extended value.
   ****************************************************************************/
  int32_t get16_sx(uint32_t addr) const;

  /**
   * @brief Reads a 32-bit value and returns it as a signed 32-bit integer.
   * @param addr The starting address to read from.
   * @return The int32_t value.
   ****************************************************************************/
  int32_t get32_sx(uint32_t addr) const;

  /**
   * @brief Writes an 8-bit value to memory.
   * @param addr The address to write to.
   * @param val The uint8_t value to write.
   ****************************************************************************/
  void set8(uint32_t addr, uint8_t val);

  /**
   * @brief Writes a 16-bit value to memory (little-endian).
   * @param addr The starting address to write to.
   * @param val The uint16_t value to write.
   ****************************************************************************/
  void set16(uint32_t addr, uint16_t val);

  /**
   * @brief Writes a 32-bit value to memory (little-endian).
   * @param addr The starting address to write to.
   * @param val The uint32_t value to write.
   ****************************************************************************/
  void set32(uint32_t addr, uint32_t val);

  /**
   * @brief Dumps the entire contents of memory to std::cout in a hex
   * and ASCII format.
   ****************************************************************************/
  void dump() const;

  /**
   * @brief Loads a binary file into the memory.
   * @param fname The path to the file to load.
   * @return true if the file was loaded successfully, false otherwise.
   * @note Stops loading if the file is larger than the memory size.
   ****************************************************************************/
  bool load_file(const std::string &fname);

private:
  std::vector<uint8_t> mem;
};
