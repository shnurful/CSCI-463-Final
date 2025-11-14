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

  case opcode_system: {
    if (insn == insn_ecall)
      return render_ecall(insn);
    else if (insn == insn_ebreak)
      return render_ebreak(insn);
    else
      return render_illegal_insn();
  }

  case opcode_rtype: {
    switch (get_funct3(insn)) {
    case funct3_add: {
      uint32_t f7 = get_funct7(insn);
      if (f7 == funct7_add)
        render_rtype(insn, "add");
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
        render_rtype(insn, "sra");
      else if (f7 == funct7_srl)
        return render_rtype(insn, "srl");
      else
        assert(0 && "unrecognized funct7");
    }
    case funct3_xor: {
      return render_rtype(insn, "xor");
    }
    default: {
      render_illegal_insn();
    }
        assert(0 && "unrecognized funct3");
    }
  }
  case opcode_alu_imm: {
    switch (get_funct3(insn)) {
    case funct3_add: {
      render_itype_alu(insn, "addi", get_imm_i(insn));
    }
    case funct3_and: {
      return render_itype_alu(insn, "andi", get_imm_i(insn));
    }
    case funct3_or: {
      return render_itype_alu(insn, "ori", get_imm_i(insn));
    }
    case funct3_sll: {
      return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);
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
        render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
      else if (f7 == funct7_srl)
        return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
      else
        assert(0 && "unrecognized funct7");
    }
    case funct3_xor: {
      return render_itype_alu(insn, "xori", get_imm_i(insn));
    }
    default: {
      render_illegal_insn();
    }
        assert(0 && "unrecognized funct3");
    }
  }
  
  case opcode_load_imm: {
    switch (get_funct3(insn)) {
    case funct3_lb: {
      render_itype_load(insn, "lb");
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
      render_illegal_insn();
    }
        assert(0 && "unrecognized funct3");
    }
  }
  default: {
    return render_illegal_insn();
  }
  }
  assert(0 && "you fucked up. oopsies!");
}

string rv32i_decode::render_illegal_insn() {
  return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

string rv32i_decode::render_lui(uint32_t insn) {
  ostringstream os;

  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  os << render_mnemonic("lui") << render_reg(rd) << "," << to_hex0x20(imm_u);
  return os.str();
}

string rv32i_decode::render_auipc(uint32_t insn) {
  ostringstream os;

  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  os << render_mnemonic("lui") << render_reg(rd) << "," << to_hex0x20(imm_u);
  return os.str();
}

string rv32i_decode::render_jal(uint32_t addr, uint32_t insn) {
  ostringstream os;

  uint32_t rd = get_rd(insn);
  uint32_t imm_j = get_imm_j(insn);

  os << render_mnemonic("jal") << render_reg(rd) << ","
     << to_hex0x32(imm_j + addr);

  return os.str();
}

string rv32i_decode::render_itype_load(uint32_t insn, const char *m){
  ostringstream os;
  int32_t imm_i = get_imm_i(insn);
  uint32_t rd = get_rd(insn);
  uint32_t r1 = get_rs1(insn);

  os << render_mnemonic(m) << render_base_disp(rd, (imm_i));

  return os.str();
}

string rv32i_decode::render_itype_alu(uint32_t insn, const char *m, int32_t imm_i){
  ostringstream os;
  uint32_t rd = get_rd(insn);
  uint32_t r1 = get_rs1(insn);

  os << render_mnemonic(m) << render_reg(rd) << "," << render_reg(r1) << imm_i;
  return os.str();
}

string rv32i_decode::render_rtype(uint32_t insn, const char *m) {
  ostringstream os;
  uint32_t rd = get_rd(insn);
  uint32_t r1 = get_rs1(insn);
  uint32_t r2 = get_rs2(insn);

  os << render_mnemonic(m) << render_reg(rd) << "," << render_reg(r1) << ","
     << render_reg(r2);

  return os.str();
}

string rv32i_decode::render_ecall(uint32_t insn) {
  return render_mnemonic("ecall");
}

string rv32i_decode::render_ebreak(uint32_t insn) {
  return render_mnemonic("ebreak");
}

uint32_t rv32i_decode::get_opcode(uint32_t insn) { return (insn & 0x0000007f); }

uint32_t rv32i_decode::get_rd(uint32_t insn) {
  uint32_t u = (insn >> (11 - 4)) & 0x1f;
  return u;
}

uint32_t rv32i_decode::get_funct3(uint32_t insn) {
  return ((insn & 0x00007000) >> (12 - 0));
}

uint32_t rv32i_decode::get_rs1(uint32_t insn) {
  return ((insn & 0x000f8000) >> (15 - 0));
}

uint32_t rv32i_decode::get_rs2(uint32_t insn) {
  return ((insn & 0x01f00000) >> (20 - 0));
}

uint32_t rv32i_decode::get_funct7(uint32_t insn) {
  return ((insn & 0xFE000000) >> (25 - 0));
}

int32_t rv32i_decode::get_imm_i(uint32_t insn) {
  int32_t i = (insn & 0xfff00000) >> (20 - 0);

  // compiler abuse, automatic sign extension on signed int
  // when sign bit is at 11 
  i = (i << 20);
  i = (i >> 20);
  return i;
}

int32_t rv32i_decode::get_imm_u(uint32_t insn) {
  // have to do this cause shifting int32 does sign extension
  uint32_t u = insn >> (12 - 0);
  return (int32_t)u;
}

int32_t rv32i_decode::get_imm_b(uint32_t insn) {
  uint32_t u = 0x000;
  return u;
}

int32_t rv32i_decode::get_imm_s(uint32_t insn) {
  int32_t s = (insn & 0xfe000000) >> (25 - 5);
  s |= (insn & 0x00000f80) >> (7 - 0);

  if (insn & 0x80000000)
    s |= 0xfffff000;

  return s;
}

int32_t rv32i_decode::get_imm_j(uint32_t insn) {
  int32_t j = (insn & 0x7FF00000) >> (21 - 1);
  j |= (insn & 0x00100000) >> (20 - 11);
  j |= (insn & 0x000ff000);
  j |= (insn & 0x80000000) >> (31 - 20);

  // compiler abuse, automatic sign extension on signed int
  // when sign bit is at 20
  j = (j << 11);
  j = (j >> 11);

  return j;
}

string rv32i_decode::render_reg(int r) { return ("x" + to_string(r)); }

string rv32i_decode::render_base_disp(uint32_t base, int32_t disp) {
  ostringstream os;
  os << to_string(disp) << "(" << render_reg(base) << ")";
  return os.str();
}

string rv32i_decode::render_mnemonic(const string &m) {
  ostringstream os;
  if (m != "ecall" && m != "ebreak")
    os << std::setw(mnemonic_width) << std::left << m;
  else
    os << std::left << m;

  return os.str();
}
