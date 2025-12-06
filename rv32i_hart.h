/* 	Ethan Silo
	z1838047
	CSCI 463-PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#pragma once
#include "memory.h"
#include "registerfile.h"
#include "rv32i_decode.h"

/**
 * @class rv32i_hart
 * @brief Class representing a RISC-V hart (hardware thread).
 *
 * This class simulates a single RISC-V hart, including registers,
 * PC, and execution logic. It inherits from rv32i_decode to handle
 * instruction decoding.
 ********************************************************************************/
class rv32i_hart : public rv32i_decode {
public:
  /**
   * @brief Constructs a new rv32i_hart object.
   * @param m Reference to the memory object to be used by the hart.
   ****************************************************************************/
  rv32i_hart(memory &m) : mem(m) {};

  /**
   * @brief Sets the flag to show instructions during execution.
   * @param b true to show instructions, false otherwise.
   ****************************************************************************/
  void set_show_instructions(bool b) { show_insns = b; };

  /**
   * @brief Sets the flag to show registers during execution.
   * @param b true to show registers, false otherwise.
   ****************************************************************************/
  void set_show_registers(bool b) { show_regs = b; };

  /**
   * @brief Checks if the hart is halted.
   * @return true if halted, false otherwise.
   ****************************************************************************/
  bool is_halted() const { return halt; };

  /**
   * @brief Gets the reason for the halt.
   * @return The string description of the halt reason.
   ****************************************************************************/
  const std ::string &get_halt_reason() const { return halt_reason; };

  /**
   * @brief Gets the number of instructions executed.
   * @return The instruction count.
   ****************************************************************************/
  uint64_t get_insn_counter() const { return insn_counter; };

  /**
   * @brief Sets the hart ID (mhartid CSR).
   * @param i The hart ID.
   ****************************************************************************/
  void set_mhartid(int i) { mhartid = i; }

  /**
   * @brief Executes a single instruction cycle.
   * @param hdr Optional header string for output.
   ****************************************************************************/
  void tick(const std::string &hdr = "");

  /**
   * @brief Dumps the current state of the hart (registers and memory).
   * @param hdr Optional header string for output.
   ****************************************************************************/
  void dump(const std::string &hdr = "") const;

  /**
   * @brief Resets the hart's state.
   ****************************************************************************/
  void reset();

protected:
  registerfile regs;
  memory &mem;

private:
  static constexpr int instruction_width = 35;
  void exec(uint32_t insn, std::ostream *);

  // misc
  void exec_illegal_insn(std::ostream *);
  void exec_lui(uint32_t insn, std::ostream *);
  void exec_auipc(uint32_t insn, std::ostream *);

  // j type
  void exec_jal(uint32_t insn, std::ostream *);
  void exec_jalr(uint32_t insn, std::ostream *);

  // opcode rtype
  void exec_add(uint32_t insn, std::ostream *);
  void exec_sub(uint32_t insn, std::ostream *);
  void exec_and(uint32_t insn, std::ostream *);
  void exec_or(uint32_t insn, std::ostream *);
  void exec_sll(uint32_t insn, std::ostream *);
  void exec_slt(uint32_t insn, std::ostream *);
  void exec_sltu(uint32_t insn, std::ostream *);
  void exec_sra(uint32_t insn, std::ostream *);
  void exec_srl(uint32_t insn, std::ostream *);
  void exec_xor(uint32_t insn, std::ostream *);

  // opcode alu imm
  void exec_addi(uint32_t insn, std::ostream *);
  void exec_andi(uint32_t insn, std::ostream *);
  void exec_ori(uint32_t insn, std::ostream *);
  void exec_slli(uint32_t insn, std::ostream *);
  void exec_slti(uint32_t insn, std::ostream *);
  void exec_sltiu(uint32_t insn, std::ostream *);
  void exec_srai(uint32_t insn, std::ostream *);
  void exec_srli(uint32_t insn, std::ostream *);
  void exec_xori(uint32_t insn, std::ostream *);

  // opcode load_imm
  void exec_lb(uint32_t insn, std::ostream *);
  void exec_lh(uint32_t insn, std::ostream *);
  void exec_lw(uint32_t insn, std::ostream *);
  void exec_lbu(uint32_t insn, std::ostream *);
  void exec_lhu(uint32_t insn, std::ostream *);

  // opcode btype
  void exec_beq(uint32_t insn, std::ostream *);
  void exec_bne(uint32_t insn, std::ostream *);
  void exec_blt(uint32_t insn, std::ostream *);
  void exec_bge(uint32_t insn, std::ostream *);
  void exec_bltu(uint32_t insn, std::ostream *);
  void exec_bgeu(uint32_t insn, std::ostream *);

  // opcode stype
  void exec_sb(uint32_t insn, std::ostream *);
  void exec_sh(uint32_t insn, std::ostream *);
  void exec_sw(uint32_t insn, std::ostream *);

  // opcode system
  void exec_ecall(std::ostream *);
  void exec_ebreak(std::ostream *);
  void exec_csrrw(uint32_t insn, std::ostream *);
  void exec_csrrs(uint32_t insn, std::ostream *);
  void exec_csrrc(uint32_t insn, std::ostream *);
  void exec_csrrwi(uint32_t insn, std::ostream *);
  void exec_csrrsi(uint32_t insn, std::ostream *);
  void exec_csrrci(uint32_t insn, std::ostream *);

  bool halt = {false};
  std::string halt_reason = {" none "};

  uint64_t insn_counter = {0};
  uint32_t pc = {0x0};
  uint32_t mhartid = {0x0};

  bool show_regs = {false};
  bool show_insns = {false};
};
