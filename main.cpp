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

struct opts_list {
  bool dump_dsasmbl = false;
  bool show_insn = false;
  uint32_t exec_limit = 0x0;
  uint32_t memory_limit = 0x100;
  bool dump_hart_pre = true;
  bool dump_hart_post = true;
};

static void usage() {
  std::cerr << "Usage : rv32i [ - d ] [ - i ] [ - r ] [ - z ] [ - l exec - "
               "limit ] [ - m hex - mem - size ] infile\n"
            << "\t-d show disassembly before program execution \n"
            << "\t-i show instruction printing during execution\n"
            << "\t-l maximum number of instructions to exec\n"
            << "\t-m specify memory size(default = 0 x100)\n"
            << "\t-r show register printing during execution\n"
            << "\t-z show a dump of the regs & memory after simulation\n";
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
  int opt;
  opts_list opts;
  while ((opt = getopt(argc, argv, "m:l:dirz")) != -1) {
    switch (opt) {
    case 'm': {
      std::istringstream iss(optarg);
      iss >> std::hex >> opts.memory_limit;
      break;
    }
    case 'l': {
      std::istringstream iss(optarg);
      iss >> std::hex >> opts.exec_limit;
      break;
    }
    case 'd': {
      opts.dump_dsasmbl = true;
      break;
    }
    case 'i': {
      opts.show_insn = true;
      break;
    }
    case 'r': {
      opts.dump_hart_pre = true;
      break;
    }
    case 'z': {
      opts.dump_hart_post = true;
      break;
    }
    default: /* ’?’ */
      usage();
    }
  }
  if (optind >= argc)
    usage(); // missing filename
  memory mem(opts.memory_limit);

  if (!mem.load_file(argv[optind]))
    usage();

  if (opts.dump_dsasmbl) {
    disassemble(mem);
    mem.dump();
  }

  return 0;
}
