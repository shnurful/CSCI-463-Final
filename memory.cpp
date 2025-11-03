/* Ethan Silo
    z1838047
    CSCI 340-PE1
    
    I certify that this is my own work and where appropriate an extension 
    of the starter code provided for the assignment.
*/
/**
 * @file memory.cpp
 * @brief Implementation of the memory class.
 *
 * Provides definitions for the memory simulation class, including
 * constructors, destructors, memory access (get/set), and file loading.
 ********************************************************************************/
#include "memory.h"
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <vector>

/**
 * @brief Constructs a new memory object.
 *
 * Initializes a vector of a given size, rounding the size up to the
 * nearest multiple of 16. All memory is initialized to 0xa5.
 *
 * @param size The desired size of the memory.
 ********************************************************************************/
memory::memory(uint32_t size) {
  size = (size + 15) & 0xfffffff0;
  mem.resize(size, 0xa5);
}

/**
 * @brief Destroys the memory object.
 *
 * Clears the internal memory vector to free resources.
 ********************************************************************************/
memory::~memory() { std::vector<uint8_t>().swap(mem); }

/**
 * @brief Gets the total allocated size of the memory.
 * @return The size of the memory in bytes, rounded.
 ********************************************************************************/
uint32_t memory::get_size() const { return (mem.size() + 15) & 0xfffffff0; }

/**
 * @brief Checks if a given address is outside the allocated memory size.
 *
 * If the address is out of bounds, it prints a warning message to std::cout.
 *
 * @param i The address to check.
 * @return true if the address is out of bounds (>= size), false otherwise.
 ********************************************************************************/
bool memory::check_illegal(uint32_t i) const {

  if (i > mem.size()) {
    std::cout << "WARNING: Address out of range: " << to_hex0x32(i)
              << std::endl;
    return true;
  }
  return false;
}

/**
 * @brief Reads an 8-bit unsigned value from memory.
 * @param addr The address to read from.
 * @return The uint8_t value at the address, or 0 if the address
 * is illegal.
 ********************************************************************************/
uint8_t memory::get8(uint32_t addr) const {
  if (check_illegal(addr) == true)
    return 0x00;

  return mem[addr];
}

/**
 * @brief Reads a 16-bit unsigned value from memory (little-endian).
 *
 * Reads from the lowest address first to ensure warnings are printed
 * in ascending order.
 *
 * @param addr The starting address to read from.
 * @return The uint16_t value, or 0 if any part of the address
 * is illegal.
 ********************************************************************************/
uint16_t memory::get16(uint32_t addr) const {
  uint16_t low, high;
  low = get8(addr);
  high = get8(addr + 1);

  return (high << 8) | low;
}

/**
 * @brief Reads a 32-bit unsigned value from memory (little-endian).
 *
 * Reads from the lowest address first to ensure warnings are printed
 * in ascending order.
 *
 * @param addr The starting address to read from.
 * @return The uint32_t value, or 0 if any part of the address
 * is illegal.
 ********************************************************************************/
uint32_t memory::get32(uint32_t addr) const {
  uint32_t low, high;
  low = get16(addr);
  high = get16(addr + 2);

  return (high << 16) | low;
}

/**
 * @brief Reads a 8-bit value and sign-extends it to 32 bits.
 *
 * Reads an unsigned 8-bit value, casts it to a signed 32-bit
 * integer, and then performs bitwise shifts to ensure the sign
 * bit (bit 7) is extended to fill the upper 24 bits.
 *
 * @param addr The address to read from.
 * @return The int32_t sign-extended value.
 ********************************************************************************/
int32_t memory::get8_sx(uint32_t addr) const {
  int32_t val = get8(addr);

  val <<= 24;
  val >>= 24;

  return val;
}

/**
 * @brief Reads a 16-bit value and sign-extends it to 32 bits.
 *
 * Reads an unsigned 16-bit value, casts it to a signed 32-bit
 * integer, and then performs bitwise shifts to ensure the sign
 * bit (bit 15) is extended to fill the upper 16 bits.
 *
 * @param addr The address to read from.
 * @return The int32_t sign-extended value.
 ********************************************************************************/
int32_t memory::get16_sx(uint32_t addr) const {
  int32_t val = get16(addr);

  val <<= 16;
  val >>= 16;

  return val;
}

/**
 * @brief Reads a 32-bit value and returns it as a signed 32-bit integer.
 *
 * This function simply calls get32() and casts the result.
 * No sign extension is performed as the value is already 32 bits.
 *
 * @param addr The address to read from.
 * @return The int32_t value.
 ********************************************************************************/
int32_t memory::get32_sx(uint32_t addr) const { return get32(addr); }

/**
 * @brief Writes an 8-bit value to memory.
 *
 * Checks if the address is legal before writing. If the address
 * is out of bounds, the write is aborted.
 *
 * @param addr The address to write to.
 * @param val The uint8_t value to write.
 ********************************************************************************/
void memory::set8(uint32_t addr, uint8_t val) {
  if (check_illegal(addr) == true)
    return;

  mem[addr] = val;
}

/**
 * @brief Writes a 16-bit value to memory (little-endian).
 *
 * The 16-bit value is split into two 8-bit bytes. The low byte
 * is written to 'addr' and the high byte is written to 'addr + 1'.
 *
 * @param addr The starting address to write to.
 * @param val The uint16_t value to write.
 ********************************************************************************/
void memory::set16(uint32_t addr, uint16_t val) {
  uint8_t start8, end8;

  start8 = uint8_t(val >> 8);
  end8 = uint8_t(val);

  set8(addr, end8);
  set8(addr + 1, start8);
}

/**
 * @brief Writes a 32-bit value to memory (little-endian).
 *
 * The 32-bit value is split into two 16-bit words. The low word
 * is written to 'addr' and the high word is written to 'addr + 2',
 * handled by set16().
 *
 * @param addr The starting address to write to.
 * @param val The uint32_t value to write.
 ********************************************************************************/
void memory::set32(uint32_t addr, uint32_t val) {
  uint16_t start16, end16;

  start16 = uint16_t(val >> 16);
  end16 = uint16_t(val);

  set16(addr, end16);
  set16(addr + 2, start16);
}

/**
 * @brief Dumps the contents of memory to stdout.
 *
 * Prints the memory contents in a hex dump format. Each line displays
 * the 32-bit starting address, followed by 16 bytes in hexadecimal,
 * followed by the ASCII representation of those 16 bytes.
 * Non-printable ASCII characters are replaced with a '.'.
 ********************************************************************************/
void memory::dump() const {
  for (uint32_t addr = 0; addr < mem.size(); addr += 16) {
    std::string ascii = "";
    std::cout << to_hex32(addr) + ":";

    for (uint32_t i = 0; i < 16 && (addr + i) < mem.size(); i++) {
      if (i == 8)
        std::cout << " ";

      std::cout << std::right << std::setw(3) << to_hex8(mem[i + addr]);

      uint8_t ch = get8(i + addr);
      ch = isprint(ch) ? ch : '.';
      ascii.append(1, ch);
    }
    std::cout << " " << "*" << ascii << "*" << std::endl;
  }
}

/**
 * @brief Loads a binary file into memory.
 *
 * Reads the contents of a binary file byte by byte and loads them
 * sequentially into the memory, starting from address 0.
 *
 * @param fname The path to the file to load.
 * @return true if the file was successfully loaded, false if the file
*        could not be opened or if the file contents exceed
*        the memory size.
 ********************************************************************************/
bool memory::load_file(const std::string &fname) {
  std::ifstream infile(fname, std::ios::in | std::ios::binary);
  if (!infile) {
    std::cerr << "Can't open file '" << fname << "' for reading.";
    return false;
  }

  uint8_t i;
  infile >> std::noskipws;
  for (uint32_t addr = 0; infile >> i; ++addr) {
    if (check_illegal(addr) == true) {
      std::cerr << "Can't open file '" << fname << "' for reading.";
      return false;
    }
    mem[addr] = i;
  }
  return true;
}
