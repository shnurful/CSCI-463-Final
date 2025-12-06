/* 	Ethan Silo
	z1838047
	CSCI 463-PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#include "cpu_single_hart.h"
#include <iostream>

/**
 * @brief Runs the execution loop for the CPU.
 *
 * This method initializes register x2 (Stack Pointer) to the memory size,
 * effectively setting the stack to grow downwards from the top of memory.
 * It then enters a loop that calls tick() to execute instructions.
 * The loop terminates when the hart is halted or when the instruction
 * counter reaches the specified execution limit (if non-zero).
 * Finally, it prints the reason for termination and the total instruction count.
 *
 * @param exec_limit The limit on the number of instructions to execute.
 * Passing 0 means no limit.
 ********************************************************************************/
void cpu_single_hart::run(uint64_t exec_limit) {
  regs.set(2, mem.get_size());
  if (exec_limit == 0x0) {
    while (!is_halted()) {
      tick();
    }
  } else {
    while (!is_halted() && get_insn_counter() != exec_limit) {
      tick();
    }
  }
  std::cout << "Execution terminated. Reason: " << get_halt_reason();
  std::cout << '\n' << get_insn_counter() << " instructions executed" << '\n';
}
