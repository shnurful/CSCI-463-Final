#pragma once
#include <cstdint>
#include <vector>
#include <string>

using std::vector;
using std::string;

/**
 * @class registerfile
 * @brief Simulates the register file of a RISC-V processor.
 *
 * This class manages the state of the 32 general-purpose registers (x0-x31).
 * It ensures that register x0 is always hardwired to 0 and provides methods
 * to read, write, and display the register contents.
 ********************************************************************************/
class registerfile {
public:
  /**
   * @brief Constructs a new registerfile object.
   *
   * Calls reset() to initialize the registers to their default values.
   ****************************************************************************/
  registerfile();

  /**
   * @brief Resets the register file state.
   *
   * Sets register x0 to 0 and all other registers (x1-x31) to 0xf0f0f0f0.
   ****************************************************************************/
  void reset();

  /**
   * @brief Writes a value to a specific register.
   *
   * If the destination register is x0 (index 0), the write operation is
   * ignored to maintain the invariant that x0 is always 0.
   *
   * @param r The register index (0-31).
   * @param val The 32-bit signed value to write.
   ****************************************************************************/
  void set(uint32_t r, int32_t val);

  /**
   * @brief Reads the value from a specific register.
   * @param r The register index (0-31).
   * @return The 32-bit signed value stored in the register.
   ****************************************************************************/
  int32_t get(uint32_t r) const;

  /**
   * @brief Dumps the contents of the register file to stdout.
   *
   * Prints the registers in rows of 8, formatted in hexadecimal.
   *
   * @param hdr An optional string to print at the beginning of each line
   * (e.g., to indicate the context like the current PC).
   ****************************************************************************/
  void dump(const string &hdr="") const;

private:
  vector<int32_t> regs = vector<int32_t>(32,0xf0f0f0f0);
  
};
