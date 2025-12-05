#include "registerfile.h"
#include "hex.h"
#include <cstdint>
#include <iomanip>
#include <iostream>


void registerfile::reset() {
  regs[0] = 0;
  std::fill(regs.begin() + 1, regs.end(), 0xf0f0f0f0);
}

registerfile::registerfile() { reset(); }

int32_t registerfile::get(uint32_t r) const { return regs[r]; }

void registerfile::set(uint32_t r, int32_t val) { regs[r] = val; }

void registerfile::dump(const string &hdr) const {
  for (uint32_t reg = 0; reg < regs.size(); reg += 8) {
    std::cout << '\n' << hdr << " x" << reg;

    for (uint32_t i = 0; i < 8 && (reg + i) < regs.size(); i++) {
      if (i == 4)
        std::cout << " ";

      std::cout << std::right << std::setw(9) << hex::to_hex32(regs[i + reg]);
    }
  }
}

