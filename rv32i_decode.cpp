/* Ethan Silo
	z1838047
	CSCI 463-PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#include "rv32i_decode.h"
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>

using std::ostringstream;
using std::string;
using std::to_string;

/**
 * @brief Decodes a 32-bit RISC-V instruction.
 *
 * Dispatches to the appropriate render function based on the opcode
 * and other fields of the instruction.
 *
 * @param addr The memory address of the instruction (for PC-relative calculations).
 * @param insn The 32-bit instruction to decode.
 * @return A string representation of the disassembled instruction.
 ********************************************************************************/
string rv32i_decode::decode(uint32_t addr, uint32_t insn) {
  switch (get_opcode(insn)) {
  case opcode_lui: {
    return render_lui(insn);
  }

  case opcode_auipc: {
    return render_auipc(insn);
  }

  case opcode_jal: {
    return render_jal(addr, insn);
  }

  case opcode_jalr: {
    return render_jalr(insn);
  }

  case opcode_system: {
    if (insn == insn_ecall)
      return render_ecall();
    else if (insn == insn_ebreak)
      return render_ebreak();
    else
      switch (get_funct3(insn)) {
      case funct3_csrrw: {
        return render_csrrx(insn, "csrrw");
      }
      case funct3_csrrs: {
        return render_csrrx(insn, "csrrs");
      }
      case funct3_csrrc: {
        return render_csrrx(insn, "csrrc");
      }
      case funct3_csrrwi: {
        return render_csrrxi(insn, "csrrwi");
      }
      case funct3_csrrsi: {
        return render_csrrxi(insn, "csrrsi");
      }
      case funct3_csrrci: {
        return render_csrrxi(insn, "csrrci");
      }
      default: {
        return render_illegal_insn();
      }
        assert(0 && "unrecognized funct3 system");
      }
    assert(0 && "system fucked");
  }

  case opcode_rtype: {
    switch (get_funct3(insn)) {
    case funct3_add: {
      uint32_t f7 = get_funct7(insn);
      if (f7 == funct7_add)
        return render_rtype(insn, "add");
      else if (f7 == funct7_sub)
        return render_rtype(insn, "sub");
      else
        assert(0 && "unrecognized funct7");
    }
    case funct3_and: {
      return render_rtype(insn, "and");
    }

    case funct3_or: {
      return render_rtype(insn, "or");
    }

    case funct3_sll: {
      return render_rtype(insn, "sll");
    }
    case funct3_slt: {
      return render_rtype(insn, "slt");
    }
    case funct3_sltu: {
      return render_rtype(insn, "sltu");
    }

    case funct3_srx: {
      uint32_t f7 = get_funct7(insn);
      if (f7 == funct7_sra)
        return render_rtype(insn, "sra");
      else if (f7 == funct7_srl)
        return render_rtype(insn, "srl");
      else
        assert(0 && "unrecognized funct7");
    }
    case funct3_xor: {
      return render_rtype(insn, "xor");
    }
    default: {
      return render_illegal_insn();
    }
      assert(0 && "unrecognized funct3 srx");
    }
  }
  case opcode_alu_imm: {
    switch (get_funct3(insn)) {
    case funct3_add: {
      return render_itype_alu(insn, "addi", get_imm_i(insn));
    }
    case funct3_and: {
      return render_itype_alu(insn, "andi", get_imm_i(insn));
    }
    case funct3_or: {
      return render_itype_alu(insn, "ori", get_imm_i(insn));
    }
    case funct3_sll: {
      return render_itype_alu(insn, "slli", get_imm_i(insn) % XLEN);
    }
    case funct3_slt: {
      return render_itype_alu(insn, "slti", get_imm_i(insn));
    }
    case funct3_sltu: {
      return render_itype_alu(insn, "sltiu", get_imm_i(insn));
    }

    case funct3_srx: {
      uint32_t f7 = (get_funct7(insn) & funct7_sra);
      if (f7 == funct7_sra)
        return render_itype_alu(insn, "srai", get_imm_i(insn) % XLEN);
      else if (f7 == funct7_srl)
        return render_itype_alu(insn, "srli", get_imm_i(insn) % XLEN);
      else
        return render_illegal_insn();
      assert(0 && "unrecognized funct7 alu_srx");
    }
    case funct3_xor: {
      return render_itype_alu(insn, "xori", get_imm_i(insn));
    }
    default: {
      return render_illegal_insn();
    }
      assert(0 && "unrecognized funct3 alu_srx");
    }
    assert(0 && "alu_imm fucked");
  }

  case opcode_load_imm: {
    switch (get_funct3(insn)) {
    case funct3_lb: {
      return render_itype_load(insn, "lb");
    }
    case funct3_lh: {
      return render_itype_load(insn, "lh");
    }
    case funct3_lw: {
      return render_itype_load(insn, "lw");
    }
    case funct3_lbu: {
      return render_itype_load(insn, "lbu");
    }
    case funct3_lhu: {
      return render_itype_load(insn, "lhu");
    }
    default: {
      return render_illegal_insn();
    }
      assert(0 && "unrecognized funct3");
    }
    assert(0 && "load_imm fucked");
  }
  case opcode_btype: {
    switch (get_funct3(insn)) {
    case funct3_beq: {
      return render_btype(addr, insn, "beq");
    }
    case funct3_bne: {
      return render_btype(addr, insn, "bne");
    }
    case funct3_blt: {
      return render_btype(addr, insn, "blt");
    }
    case funct3_bge: {
      return render_btype(addr, insn, "bge");
    }
    case funct3_bltu: {
      return render_btype(addr, insn, "bltu");
    }
    case funct3_bgeu: {
      return render_btype(addr, insn, "bgeu");
    }
    default:
      return render_illegal_insn();
      assert(0 && "unrecognized funct3");
    }
    assert(0 && "btype fucked");
  }
  case opcode_stype: {
    switch (get_funct3(insn)) {
    case funct3_sb: {
      return render_stype(insn, "sb");
    }
    case funct3_sh: {
      return render_stype(insn, "sh");
    }
    case funct3_sw: {
      return render_stype(insn, "sw");
    }
    default:
      return render_illegal_insn();
      assert(0 && "unrecognized funct3 stype");
    }
    assert(0 && "stype fucked");
  }
  default: {
    return render_illegal_insn();
  }
  }
  assert(0 && "you fucked up. oopsies!");
}

/**
 * @brief Returns an error string for illegal instructions.
 * @return String "ERROR: UNIMPLEMENTED INSTRUCTION".
 ********************************************************************************/
string rv32i_decode::render_illegal_insn() {
  return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * @brief Decodes the LUI (Load Upper Immediate) instruction.
 * @param insn The instruction to decode.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_lui(uint32_t insn) {
  ostringstream os;

  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  os << render_mnemonic("lui") << render_reg(rd) << "," << to_hex0x20(imm_u);
  return os.str();
}

/**
 * @brief Decodes the AUIPC (Add Upper Immediate to PC) instruction.
 * @param insn The instruction to decode.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_auipc(uint32_t insn) {
  ostringstream os;

  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  os << render_mnemonic("auipc") << render_reg(rd) << "," << to_hex0x20(imm_u);
  return os.str();
}

/**
 * @brief Decodes the JAL (Jump and Link) instruction.
 * @param addr The address of the instruction.
 * @param insn The instruction to decode.
 * @return The formatted assembly string with target address.
 ********************************************************************************/
string rv32i_decode::render_jal(uint32_t addr, uint32_t insn) {
  ostringstream os;

  uint32_t rd = get_rd(insn);
  uint32_t imm_j = get_imm_j(insn);

  os << render_mnemonic("jal") << render_reg(rd) << ","
     << to_hex0x32(imm_j + addr);

  return os.str();
}

/**
 * @brief Decodes the JALR (Jump and Link Register) instruction.
 * @param insn The instruction to decode.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_jalr(uint32_t insn) {
  ostringstream os;
  uint32_t rd = get_rd(insn);
  uint32_t r1 = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  os << render_mnemonic("jalr") << render_reg(rd) << ","
     << render_base_disp(r1, imm_i);
  return os.str();
}

/**
 * @brief Decodes Branch-type instructions (e.g., BEQ, BNE).
 * @param addr The address of the instruction.
 * @param insn The instruction to decode.
 * @param m The mnemonic string.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *m) {
  ostringstream os;
  uint32_t r1 = get_rs1(insn);
  uint32_t r2 = get_rs2(insn);
  uint32_t imm_b = get_imm_b(insn);
  uint32_t pcrel = imm_b + addr;

  os << render_mnemonic(m) << render_reg(r1) << "," << render_reg(r2) << ","
     << to_hex0x32(pcrel);

  return os.str();
}

/**
 * @brief Decodes I-type Load instructions (e.g., LW, LB).
 * @param insn The instruction to decode.
 * @param m The mnemonic string.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_itype_load(uint32_t insn, const char *m) {
  ostringstream os;
  int32_t imm_i = get_imm_i(insn);
  uint32_t rd = get_rd(insn);
  uint32_t r1 = get_rs1(insn);

  os << render_mnemonic(m) << render_reg(rd) << ","
     << render_base_disp(r1, (imm_i));

  return os.str();
}

/**
 * @brief Decodes S-type Store instructions (e.g., SW, SB).
 * @param insn The instruction to decode.
 * @param m The mnemonic string.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_stype(uint32_t insn, const char *m) {
  ostringstream os;
  uint32_t r1 = get_rs1(insn);
  uint32_t r2 = get_rs2(insn);
  int32_t imm_s = get_imm_s(insn);

  os << render_mnemonic(m) << render_reg(r2) << ","
     << render_base_disp(r1, imm_s);

  return os.str();
}

/**
 * @brief Decodes I-type ALU instructions (e.g., ADDI, ANDI).
 * @param insn The instruction to decode.
 * @param m The mnemonic string.
 * @param imm_i The immediate value.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_itype_alu(uint32_t insn, const char *m,
                                      int32_t imm_i) {
  ostringstream os;
  uint32_t rd = get_rd(insn);
  uint32_t r1 = get_rs1(insn);

  os << render_mnemonic(m) << render_reg(rd) << "," << render_reg(r1) << ","
     << imm_i;
  return os.str();
}

/**
 * @brief Decodes R-type instructions (e.g., ADD, SUB).
 * @param insn The instruction to decode.
 * @param m The mnemonic string.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_rtype(uint32_t insn, const char *m) {
  ostringstream os;
  uint32_t rd = get_rd(insn);
  uint32_t r1 = get_rs1(insn);
  uint32_t r2 = get_rs2(insn);

  os << render_mnemonic(m) << render_reg(rd) << "," << render_reg(r1) << ","
     << render_reg(r2);

  return os.str();
}

/**
 * @brief Decodes the ECALL instruction.
 * @return "ecall" string.
 ********************************************************************************/
string rv32i_decode::render_ecall() { return render_mnemonic("ecall"); }

/**
 * @brief Decodes the EBREAK instruction.
 * @return "ebreak" string.
 ********************************************************************************/
string rv32i_decode::render_ebreak() { return render_mnemonic("ebreak"); }

/**
 * @brief Decodes CSR instructions (CSRRW, CSRRS, CSRRC).
 * @param insn The instruction to decode.
 * @param m The mnemonic string.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_csrrx(uint32_t insn, const char *m) {
  ostringstream os;
  uint32_t rd = get_rd(insn);
  uint32_t r1 = get_rs1(insn);
  int32_t csr = get_imm_i(insn);

  os << render_mnemonic(m) << render_reg(rd) << "," << to_hex0x12(csr & 0xfff)
     << "," << render_reg(r1);
  return os.str();
}

/**
 * @brief Decodes CSR instructions with immediate operands (CSRRWI, CSRRSI, CSRRCI).
 * @param insn The instruction to decode.
 * @param m The mnemonic string.
 * @return The formatted assembly string.
 ********************************************************************************/
string rv32i_decode::render_csrrxi(uint32_t insn, const char *m) {
  ostringstream os;
  uint32_t rd = get_rd(insn);
  uint32_t zimm = get_rs1(insn);
  uint32_t csr = get_imm_i(insn);
  csr &= 0x0fff; // gotta remask cause imm_i is signed

  os << render_mnemonic(m) << render_reg(rd) << "," << to_hex0x12(csr) << ","
     << zimm;
  return os.str();
}

/**
 * @brief Extracts the opcode (bits 0-6).
 * @param insn The instruction.
 * @return The opcode value.
 ********************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn) { return (insn & 0x0000007f); }

/**
 * @brief Extracts the destination register (bits 7-11).
 * @param insn The instruction.
 * @return The register index.
 ********************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn) {
  uint32_t u = (insn >> (11 - 4)) & 0x1f;
  return u;
}

/**
 * @brief Extracts the funct3 field (bits 12-14).
 * @param insn The instruction.
 * @return The funct3 value.
 ********************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn) {
  return ((insn & 0x00007000) >> (12 - 0));
}

/**
 * @brief Extracts the source register 1 (bits 15-19).
 * @param insn The instruction.
 * @return The register index.
 ********************************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn) {
  return ((insn & 0x000f8000) >> (15 - 0));
}

/**
 * @brief Extracts the source register 2 (bits 20-24).
 * @param insn The instruction.
 * @return The register index.
 ********************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn) {
  return ((insn & 0x01f00000) >> (20 - 0));
}

/**
 * @brief Extracts the funct7 field (bits 25-31).
 * @param insn The instruction.
 * @return The funct7 value.
 ********************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn) {
  return ((insn & 0xFE000000) >> (25 - 0));
}

/**
 * @brief Extracts the I-type immediate.
 *
 * Performs sign-extension.
 * @param insn The instruction.
 * @return The sign-extended immediate.
 ********************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn) {
  int32_t i = (insn & 0xfff00000) >> (20 - 0);

  // compiler abuse, automatic sign extension on signed int
  // when sign bit is at 11
  i = (i << 20);
  i = (i >> 20);
  return i;
}

/**
 * @brief Extracts the U-type immediate.
 * @param insn The instruction.
 * @return The immediate value.
 ********************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn) {
  // have to do this cause shifting int32 does sign extension
  uint32_t u = insn >> (12 - 0);
  return (int32_t)u;
}

/**
 * @brief Extracts the B-type immediate.
 *
 * Reconstructs the immediate from split fields and performs sign-extension.
 * @param insn The instruction.
 * @return The sign-extended immediate.
 ********************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn) {
  int32_t b = (insn & 0x00000f00) >> (8 - 1);
  b |= (insn & 0x7e000000) >> (25 - 5);
  b |= (insn & 0x00000080) << (11 - 7);
  b |= (insn & 0x80000000) >> (31 - 12);

  // compiler abuse, automatic sign extension on signed int
  // when sign bit is at 20
  b = (b << (31 - 12));
  b = (b >> (31 - 12));

  return b;
}

/**
 * @brief Extracts the S-type immediate.
 *
 * Reconstructs the immediate from split fields and performs sign-extension.
 * @param insn The instruction.
 * @return The sign-extended immediate.
 ********************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn) {
  int32_t s = (insn & 0xfe000000) >> (25 - 5);
  s |= (insn & 0x00000f80) >> (7 - 0);

  if (insn & 0x80000000)
    s |= 0xfffff000;

  return s;
}

/**
 * @brief Extracts the J-type immediate.
 *
 * Reconstructs the immediate from split fields and performs sign-extension.
 * @param insn The instruction.
 * @return The sign-extended immediate.
 ********************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn) {
  int32_t j = (insn & 0x7fe00000) >> (21 - 1);
  j |= (insn & 0x00100000) >> (20 - 11);
  j |= (insn & 0x000ff000);
  j |= (insn & 0x80000000) >> (31 - 20);

  // compiler abuse, automatic sign extension on signed int
  // when sign bit is at 20
  j = (j << (31 - 20));
  j = (j >> (31 - 20));

  return j;
}

/**
 * @brief Formats the register name (e.g., x1).
 * @param r The register index.
 * @return String representation of the register.
 ********************************************************************************/
string rv32i_decode::render_reg(int r) { return ("x" + to_string(r)); }

/**
 * @brief Formats the base and displacement string.
 * @param base The base register.
 * @param disp The displacement value.
 * @return Formatted string (e.g., "-4(x2)").
 ********************************************************************************/
string rv32i_decode::render_base_disp(uint32_t base, int32_t disp) {
  ostringstream os;
  os << to_string(disp) << "(" << render_reg(base) << ")";
  return os.str();
}

/**
 * @brief Formats the instruction mnemonic with padding.
 * @param m The mnemonic string.
 * @return The padded mnemonic.
 ********************************************************************************/
string rv32i_decode::render_mnemonic(const string &m) {
  ostringstream os;
  if (m != "ecall" && m != "ebreak")
    os << std::setw(mnemonic_width) << std::left << m;
  else
    os << std::left << m;

  return os.str();
}
