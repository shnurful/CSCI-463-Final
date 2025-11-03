#include "memory.h"
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <vector>
#include <fstream>


memory::memory(uint32_t size) {
  size = (size + 15) & 0xfffffff0;
  mem.resize(size, 0xa5);
}

memory::~memory() { std::vector<uint8_t>().swap(mem); }

bool memory::check_illegal(uint32_t i) const {

  if (i > mem.capacity()) {
    std::cout << "WARNING: Address out of range: " << to_hex0x32(i)
              << std::endl;
    return true;
  }
  return false;
}

uint32_t memory::get_size() const { return (mem.size() + 15) & 0xfffffff0; }

uint8_t memory::get8(uint32_t addr) const {
  if (check_illegal(addr) == true)
    return 0x00;

  return mem[addr];
}

uint16_t memory::get16(uint32_t addr) const {
  uint16_t val = 0x0000;

  val += get8(addr + 1);
  val <<= 8;
  val += get8(addr);

  return val;
}

uint32_t memory::get32(uint32_t addr) const {
  uint32_t val = 0x00000000;

  val += get16(addr + 2);
  val <<= 16;
  val += get16(addr);
  

  return val;
}

int32_t memory::get8_sx(uint32_t addr) const {
  int32_t val = 0x00000000;
  uint8_t num, msb;
  num = msb = get8(addr);
  msb >>= 7;

  if (msb == 0x01) {
    val |= 0xffffff00;
    val += num;
  } else {
    val += num;
  }

  return val;
}

int32_t memory::get16_sx(uint32_t addr) const {
  int32_t val = 0x00000000;
  uint16_t num, msb;
  num = msb = get16(addr);
  msb >>= 15;

  if (msb == 0x01) {
    val |= 0xffff0000;
    val += num;
  } else {
    val += num;
  }

  return val;
}

int32_t memory::get32_sx(uint32_t addr) const { return get32(addr); }

void memory::set8(uint32_t addr, uint8_t val) {
  if (check_illegal(addr) == true)
    return;

  mem[addr] = val;
}

void memory::set16(uint32_t addr, uint16_t val) {
  uint8_t start8, end8;

  start8 = uint8_t(val >> 8);
  end8 = uint8_t(val);

  set8(addr, end8);
  set8(addr + 1, start8);
}

void memory::set32(uint32_t addr, uint32_t val) {
  uint16_t start16, end16;

  start16 = uint16_t(val >> 16);
  end16 = uint16_t(val);

  set16(addr, end16);
  set16(addr + 2, start16);
}

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

bool memory::load_file(const std::string &fname){
  std::ifstream infile(fname, std::ios::in|std::ios::binary);
  if(!infile){
    std::cerr << "Can't open file '" << fname << "' for reading.";
    return false;
  }

  uint8_t i;
  infile >> std::noskipws;
  for(uint32_t addr=0; infile >> i; ++addr){
    if(check_illegal(addr) == true){
      std::cerr << "Can't open file '" << fname << "' for reading.";
      return false;
    }
    mem[addr] = i;
  }
  return true;
}
