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
  std::cout << insn;
  switch (get_opcode(insn)) {
  case opcode_lui:
    return render_lui(insn);
  case opcode_auipc:
    return render_auipc(insn);
  case opcode_jal:
    return render_jal(addr, insn);
  case insn_ebreak:
    return render_ebreak(insn);
  // case insn_ecall:
  //   return render_ecall(insn);
  default:
    return render_illegal_insn();
  }
  assert(0 && "oopsies! you fucked up");
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
  uint32_t u = 0x000;
  return u;
}

uint32_t rv32i_decode::get_rs1(uint32_t insn) {
  uint32_t u = 0x000;
  return u;
}

uint32_t rv32i_decode::get_rs2(uint32_t insn) {
  uint32_t u = 0x000;
  return u;
}

uint32_t rv32i_decode::get_funct7(uint32_t insn) {
  uint32_t u = 0x000;
  return u;
}

int32_t rv32i_decode::get_imm_i(uint32_t insn) {
  uint32_t u = 0x000;
  return u;
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
  return (render_reg(base) + "," + to_string(base));
}

string rv32i_decode::render_mnemonic(const string &m) {
  ostringstream os;
  if (m != "ecall" && m != "ebreak")
    os << std::setw(mnemonic_width) << std::left << m;
  else
    os << std::left << m;

  return os.str();
}
