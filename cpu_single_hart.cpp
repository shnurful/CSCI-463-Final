#include "cpu_single_hart.h"
#include <iostream>
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
  std::cout << "\nExecution terminated. Reason: " << get_halt_reason();
  std::cout << '\n' << std::hex << get_insn_counter() << " instructions executed";
}
