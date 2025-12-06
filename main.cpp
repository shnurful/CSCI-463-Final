/* Ethan Silo
    z1838047
    CSCI 463-PE1

    I certify that this is my own work and where appropriate an extension
    of the starter code provided for the assignment.
*/
/**
 * @file main.cpp
 * @brief Main entry point for the RISC-V simulator.
 *
 * This file contains the main function which handles command-line argument
 * parsing, memory initialization, loading the binary file, and starting
 * the simulation or disassembly process.
 ********************************************************************************/
#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include "cpu_single_hart.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <unistd.h>

/**
 * @struct opts_list
 * @brief Structure to hold command-line options.
 *
 * Stores the configuration settings determined by command-line arguments,
 * such as memory limits, execution limits, and debug flags.
 ********************************************************************************/
struct opts_list {
  bool dump_dsasmbl = false;      //dump memory before execution
  bool show_insn = false;         //show instructions during execution
  uint64_t exec_limit = 0x0;      //max number of instructions to execute
  uint32_t memory_limit = 0x100;  //size of memory
  bool dump_on_exec = false;       //show regs and pc before each execution
  bool dump_hart_post = false;     //show regs, pc, and memory after halt
};

/**
 * @brief Prints the usage instructions and exits.
 *
 * Displays the valid command-line arguments and their descriptions to stderr,
 * then terminates the program with exit code 1.
 ********************************************************************************/
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

/**
 * @brief Disassembles the instructions in memory.
 *
 * Iterates through the memory range and decodes the 32-bit words into
 * readable RISC-V assembly instructions, printing them to stdout.
 *
 * @param mem Reference to the memory object containing the binary code.
 ********************************************************************************/
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

/**
 * @brief Main execution function.
 *
 * Parses command-line arguments to configure the simulator. It initializes
 * memory, loads the input binary file, and optionally disassembles the code
 * or runs the simulation using the cpu_single_hart class.
 *
 * @param argc Argument count.
 * @param argv Argument values.
 * @return Returns 0 on success, or exits with 1 on error (via usage()).
 ********************************************************************************/
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
      opts.dump_on_exec = true;
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
  }
 
  cpu_single_hart cpu(mem);
  cpu.set_show_instructions(opts.show_insn);
  cpu.set_show_registers(opts.dump_on_exec);
  
  cpu.run(opts.exec_limit);

  if (opts.dump_hart_post) {
    cpu.dump();  
  }
  return 0;
}
