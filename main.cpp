/* Ethan Silo
    z1838047
    CSCI 340-PE1

    I certify that this is my own work and where appropriate an extension
    of the starter code provided for the assignment.
*/
#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <unistd.h>

static void usage() {
  std::cerr << " Usage : rv32i [ - m hex - mem - size ] infile " << std::endl;
  std::cerr << " -m specify memory size ( default = 0 x100 )" << std::endl;
  exit(1);
}
static void disassemble(const memory &mem) {
  // should end up looking like the dump loop, works on 32 bit words

  for (uint32_t addr = 0; addr < mem.get_size(); addr += 4) {
    std::cout << hex::to_hex32(addr) + ":";
    uint32_t inst = mem.get32(addr);

    std::cout << std::right << std::setw(11) << (hex::to_hex32(inst) + "  ");
    std::cout << rv32i_decode::decode(addr, inst) << std::endl;

  }
  return;
}

int main(int argc, char **argv) {
  uint32_t memory_limit = 0x100; // default memory size = 256 bytes
  int opt;
  while ((opt = getopt(argc, argv, "m:")) != -1) {
    switch (opt) {
    case 'm': {
      std::istringstream iss(optarg);
      iss >> std::hex >> memory_limit;
    } break;
    default: /* ’?’ */
      usage();
    }
  }
  if (optind >= argc)
    usage(); // missing filename
  memory mem(memory_limit);
  if (!mem.load_file(argv[optind]))
    usage();
  disassemble(mem);
  mem.dump();
  return 0;
}
