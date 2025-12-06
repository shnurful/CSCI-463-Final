/* 	Ethan Silo
	z1838047
	CSCI 463-PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#include "registerfile.h"
#include "hex.h"
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

/**
 * @brief Resets the register file.
 *
 * Register x0 is set to 0. All other registers are initialized to 0xf0f0f0f0
 * to help debug uninitialized variable usage.
 ********************************************************************************/
void registerfile::reset() {
  regs[0] = 0;
  std::fill(regs.begin() + 1, regs.end(), 0xf0f0f0f0);
}

/**
 * @brief Constructs a registerfile object and resets it.
 ********************************************************************************/
registerfile::registerfile() { reset(); }

/**
 * @brief Gets the value of a register.
 * @param r The register index (0-31).
 * @return The value held in register r.
 ********************************************************************************/
int32_t registerfile::get(uint32_t r) const { return regs[r]; }

/**
 * @brief Sets the value of a register.
 *
 * This function enforces the RISC-V rule that x0 is hardwired to 0.
 * Writes to register 0 are silently ignored.
 *
 * @param r The register index (0-31).
 * @param val The value to write into the register.
 ********************************************************************************/
void registerfile::set(uint32_t r, int32_t val) {
  if (r == 0) return;
  regs[r] = val; 
}

/**
 * @brief Dumps the register file contents to the standard output.
 *
 * The output is formatted with the register name (e.g., x0, x8) followed by
 * its hexadecimal value. 8 registers are printed per line.
 *
 * @param hdr A string prefix for each line of output.
 ********************************************************************************/
void registerfile::dump(const string &hdr) const {
  for (uint32_t reg = 0; reg < regs.size(); reg += 8) {
    if (reg != 0)
      std::cout << '\n';

    std::string rname = "x" + std::to_string(reg);
    std::cout << hdr << std::setw(3) << std::right << rname;

    for (uint32_t i = 0; i < 8 && (reg + i) < regs.size(); i++) {
      if (i == 4)
        std::cout << " ";

      std::cout << std::right << std::setw(9) << hex::to_hex32(regs[i + reg]);
    }
  }
}
