#pragma once
#include "rv32i_hart.h"

/**
 * @class cpu_single_hart
 * @brief A subclass of rv32i_hart that simulates a single-core CPU execution.
 *
 * This class adds the functionality to run the simulation loop until the
 * program halts or a specific instruction limit is reached.
 ********************************************************************************/
class cpu_single_hart : public rv32i_hart{
public:
  /**
   * @brief Constructs a new cpu_single_hart object.
   * @param mem Reference to the memory object to be used by the CPU.
   ****************************************************************************/
  cpu_single_hart(memory &mem) : rv32i_hart(mem){}

  /**
   * @brief Runs the CPU simulation.
   *
   * Initializes the stack pointer (x2) and executes instructions until
   * the processor halts or the execution limit is reached.
   *
   * @param exec_limit The maximum number of instructions to execute.
   * If 0, the simulation runs until a halt condition occurs.
   ****************************************************************************/
  void run(uint64_t exec_limit);
};
