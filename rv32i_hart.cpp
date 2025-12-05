#include "rv32i_hart.h"
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>

void rv32i_hart::exec(uint32_t insn, std::ostream *pos) {
  switch (get_opcode(insn)) {
  case opcode_lui: {
    exec_lui(insn, pos);
    return;
  }

  case opcode_auipc: {
    exec_auipc(insn, pos);
    return;
  }

  case opcode_jal: {
    exec_jal(insn, pos);
    return;
  }

  case opcode_jalr: {
    exec_jalr(insn, pos);
    return;
  }

  case opcode_system: {
    if (insn == insn_ecall)
      exec_ecall(pos);
    else if (insn == insn_ebreak)
      exec_ebreak(pos);
    else
      switch (get_funct3(insn)) {
      case funct3_csrrw: {
        exec_csrrw(insn, pos);
        return;
      }
      case funct3_csrrs: {
        exec_csrrs(insn, pos);
        return;
      }
      case funct3_csrrc: {
        exec_csrrc(insn, pos);
        return;
      }
      case funct3_csrrwi: {
        exec_csrrwi(insn, pos);
        return;
      }
      case funct3_csrrsi: {
        exec_csrrsi(insn, pos);
        return;
      }
      case funct3_csrrci: {
        exec_csrrci(insn, pos);
        return;
      }
      default: {
        exec_illegal_insn(pos);
        return;
      }
        assert(0 && "unrecognized funct3 system");
      }
    assert(0 && "system fucked");
  }

  case opcode_rtype: {
    switch (get_funct3(insn)) {
    case funct3_add: {
      uint32_t f7 = get_funct7(insn);
      if (f7 == funct7_add) {
        exec_add(insn, pos);
        return;
      } else if (f7 == funct7_sub) {
        exec_sub(insn, pos);
        return;
      } else
        assert(0 && "unrecognized funct7");
    }
    case funct3_and: {
      exec_and(insn, pos);
      return;
    }

    case funct3_or: {
      exec_or(insn, pos);
      return;
    }

    case funct3_sll: {
      exec_sll(insn, pos);
      return;
    }
    case funct3_slt: {
      exec_slt(insn, pos);
      return;
    }
    case funct3_sltu: {
      exec_sltu(insn, pos);
      return;
    }

    case funct3_srx: {
      uint32_t f7 = get_funct7(insn);
      if (f7 == funct7_sra) {
        exec_sra(insn, pos);
        return;
      } else if (f7 == funct7_srl) {
        exec_srl(insn, pos);
        return;
      } else {
        exec_illegal_insn(pos);
        return;
      }
      assert(0 && "unrecognized funct7");
    }
    case funct3_xor: {
      exec_xor(insn, pos);
      return;
    }
    default: {
      exec_illegal_insn(pos);
      return;
    }
      assert(0 && "unrecognized funct3 srx");
    }
  }
  case opcode_alu_imm: {
    switch (get_funct3(insn)) {
    case funct3_add: {
      exec_addi(insn, pos);
      return;
    }
    case funct3_and: {
      exec_andi(insn, pos);
      return;
    }
    case funct3_or: {
      exec_ori(insn, pos);
      return;
    }
    case funct3_sll: {
      exec_slli(insn, pos);
      return;
    }
    case funct3_slt: {
      exec_slti(insn, pos);
      return;
    }
    case funct3_sltu: {
      exec_sltiu(insn, pos);
      return;
    }

    case funct3_srx: {
      uint32_t f7 = (get_funct7(insn) & funct7_sra);
      if (f7 == funct7_sra) {
        exec_srai(insn, pos);
        return;
      } else if (f7 == funct7_srl) {
        exec_srli(insn, pos);
        return;
      } else {
        exec_illegal_insn(pos);
        return;
      }
      assert(0 && "unrecognized funct7 alu_srx");
    }
    case funct3_xor: {
      exec_xori(insn, pos);
      return;
    }
    default: {
      exec_illegal_insn(pos);
      return;
    }
      assert(0 && "unrecognized funct3 alu_srx");
    }
    assert(0 && "alu_imm fucked");
  }

  case opcode_load_imm: {
    switch (get_funct3(insn)) {
    case funct3_lb: {
      exec_lb(insn, pos);
      return;
    }
    case funct3_lh: {
      exec_lh(insn, pos);
      return;
    }
    case funct3_lw: {
      exec_lw(insn, pos);
      return;
    }
    case funct3_lbu: {
      exec_lbu(insn, pos);
      return;
    }
    case funct3_lhu: {
      exec_lhu(insn, pos);
      return;
    }
    default: {
      exec_illegal_insn(pos);
      return;
    }
      assert(0 && "unrecognized funct3");
    }
    assert(0 && "load_imm fucked");
  }
  case opcode_btype: {
    switch (get_funct3(insn)) {
    case funct3_beq: {
      exec_beq(insn, pos);
      return;
    }
    case funct3_bne: {
      exec_bne(insn, pos);
      return;
    }
    case funct3_blt: {
      exec_blt(insn, pos);
      return;
    }
    case funct3_bge: {
      exec_bge(insn, pos);
      return;
    }
    case funct3_bltu: {
      exec_bltu(insn, pos);
      return;
    }
    case funct3_bgeu: {
      exec_bgeu(insn, pos);
      return;
    }
    default:
      exec_illegal_insn(pos);
      return;
      assert(0 && "unrecognized funct3");
    }
    assert(0 && "btype fucked");
  }
  case opcode_stype: {
    switch (get_funct3(insn)) {
    case funct3_sb: {
      exec_sb(insn, pos);
      return;
    }
    case funct3_sh: {
      exec_sh(insn, pos);
      return;
    }
    case funct3_sw: {
      exec_sw(insn, pos);
      return;
    }
    default:
      exec_illegal_insn(pos);
      return;
      assert(0 && "unrecognized funct3 stype");
    }
    assert(0 && "stype fucked");
  }
  default: {
    exec_illegal_insn(pos);
    return;
  }
  }
  assert(0 && "you fucked up. oopsies!");
}

void rv32i_hart::exec_illegal_insn(std::ostream *pos) {
  if (pos)
    *pos << render_illegal_insn();
  halt = true;
  halt_reason = "Illegal instruction";
}

void rv32i_hart::tick(const string &hdr) {
  if (halt == true)
    return;

  if (show_regs) {
    regs.dump();
    std::cout << "\npc " << to_hex32(pc);
  }

  int32_t pc_check = pc % 4;
  if (pc_check != 0) {
    halt = true;
    halt_reason = "PC alignment error";
    return;
  }

  ++insn_counter;
  int32_t insn = mem.get32(pc);

  if (show_insns) {
    std::cout << '\n' << hdr << to_hex32(pc) << ": ";
    exec(insn, &std::cout);
  } else
    exec(insn, nullptr);
}

void rv32i_hart::exec_lui(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t imm_u = get_imm_u(insn);
  imm_u <<= (0 + 12);
  if (pos) {
    string s = render_lui(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(imm_u) << ", "
         << "pc = (" << to_hex0x32(pc) << " + " << to_hex0x32(4) << ")";
  }

  regs.set(rd, imm_u);
  pc += 4;
}

void rv32i_hart::exec_auipc(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t imm_u = get_imm_u(insn);
  imm_u <<= (0 + 12);
  if (pos) {
    string s = render_auipc(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(pc) << " + "
         << to_hex0x32(imm_u) << ", "
         << "pc = (" << to_hex0x32(pc) << " + " << to_hex0x32(imm_u) << ")";
  }

  regs.set(rd, (pc + imm_u));
  pc += 4;
}

void rv32i_hart::exec_jal(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  int32_t imm_j = get_imm_j(insn);

  if (pos) {
    string s = render_jal(pc, insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(pc + 4) << ", "
         << "pc = (" << to_hex0x32(pc) << " + " << to_hex0x32(imm_j) << ")";
  }

  regs.set(rd, (pc + 0x4));
  pc = pc + imm_j;
}

void rv32i_hart::exec_jalr(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t r1 = get_rs1(insn);
  uint32_t imm_i = get_imm_i(insn);

  if (pos) {
    string s = render_jalr(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(pc + 4) << ", "
         << "pc = (" << to_hex0x32(pc) << " + " << to_hex0x32(regs.get(r1))
         << ") & " << to_hex0x32(0xfffffffe) << " = "
         << to_hex0x32(imm_i + regs.get(r1));
  }

  regs.set(rd, (pc + 4));
  pc = regs.get(r1) + imm_i;
}

void rv32i_hart::exec_ebreak(std::ostream *pos) {

  if (pos) {
    string s = render_ebreak();
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// HALT";
  }
  halt = true;
  halt_reason = "EBREAK instruction";
}
void rv32i_hart::exec_ecall(std::ostream *pos) {

  if (pos) {
    string s = render_ecall();
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// HALT";
  }
  halt = true;
  halt_reason = "ECALL instruction";
}

#define CSR_OP(NAME)                                                           \
  void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) {             \
    uint32_t rd = get_rd(insn);                                                \
    int32_t csr_addr = get_imm_i(insn);                                        \
    csr_addr <<= 20;                                                            \
                                                                               \
        uint32_t old_csr_val = 0;                                              \
    if (csr_addr == 0xf14) {                                                   \
      old_csr_val = mhartid;                                                   \
    } else {                                                                   \
      halt = true;                                                             \
      halt_reason = "Illegal CSR in CSRRS instruction";                        \
    }                                                                          \
                                                                               \
    if (csr_addr == 0xf14) {                                                   \
    }                                                                          \
                                                                               \
    if (pos) {                                                                 \
      std::string s = render_csrrx(insn, #NAME);                               \
      *pos << std::setw(instruction_width) << std::setfill(' ') << std::left   \
           << s;                                                               \
      *pos << "// " << render_reg(rd) << " = " << old_csr_val;                 \
    }                                                                          \
    regs.set(rd, old_csr_val);                                                 \
    pc += 4;                                                                   \
  }

CSR_OP(csrrw)
CSR_OP(csrrs)
CSR_OP(csrrc)

CSR_OP(csrrwi)
CSR_OP(csrrsi)
CSR_OP(csrrci)

#undef CSR_OP

#define R_TYPE_ALU(NAME, OP, TYPE, MNEMONIC)                                   \
  void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) {             \
    uint32_t rd = get_rd(insn);                                                \
    uint32_t rs1 = get_rs1(insn);                                              \
    uint32_t rs2 = get_rs2(insn);                                              \
    TYPE val1 = (TYPE)regs.get(rs1);                                           \
    TYPE val2 = (TYPE)regs.get(rs2);                                           \
    int32_t result = (int32_t)(val1 OP val2);                                  \
    if (pos) {                                                                 \
      std::string s = render_rtype(insn, MNEMONIC);                            \
      *pos << std::setw(instruction_width) << std::setfill(' ') << std::left   \
           << s;                                                               \
      *pos << "// " << render_reg(rd) << " = " << to_hex0x32(val1)             \
           << " " #OP " " << to_hex0x32(val2) << " = " << to_hex0x32(result);  \
    }                                                                          \
    regs.set(rd, result);                                                      \
    pc += 4;                                                                   \
  }

#define R_TYPE_SHIFT(NAME, OP, TYPE, MNEMONIC)                                 \
  void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) {             \
    uint32_t rd = get_rd(insn);                                                \
    uint32_t rs1 = get_rs1(insn);                                              \
    uint32_t rs2 = get_rs2(insn);                                              \
    TYPE val1 = (TYPE)regs.get(rs1);                                           \
    uint32_t amount = regs.get(rs2) & 0x1f;                                    \
    int32_t result = (int32_t)(val1 OP amount);                                \
    if (pos) {                                                                 \
      std::string s = render_rtype(insn, MNEMONIC);                            \
      *pos << std::setw(instruction_width) << std::setfill(' ') << std::left   \
           << s;                                                               \
      *pos << "// " << render_reg(rd) << " = " << to_hex0x32(val1)             \
           << " " #OP " " << std::dec << amount << " = "                       \
           << to_hex0x32(result);                                              \
    }                                                                          \
    regs.set(rd, result);                                                      \
    pc += 4;                                                                   \
  }

R_TYPE_ALU(add, +, int32_t, "add")
R_TYPE_ALU(sub, -, int32_t, "sub")
R_TYPE_ALU(and, &, int32_t, "and")
R_TYPE_ALU(or, |, int32_t, "or")
R_TYPE_ALU(xor, ^, int32_t, "xor")

R_TYPE_ALU(slt, <, int32_t, "slt")
R_TYPE_ALU(sltu, <, uint32_t, "sltu")

R_TYPE_SHIFT(sll, <<, uint32_t, "sll")
R_TYPE_SHIFT(srl, >>, uint32_t, "srl")
R_TYPE_SHIFT(sra, >>, int32_t, "sra")

#undef R_TYPE_ALU
#undef R_TYPE_SHIFT

#define ALU_IMM(NAME, OP, TYPE)                                                \
  void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) {             \
    uint32_t rd = get_rd(insn);                                                \
    uint32_t rs1 = get_rs1(insn);                                              \
    int32_t imm_i = get_imm_i(insn);                                           \
    TYPE val1 = (TYPE)regs.get(rs1);                                           \
    TYPE val2 = (TYPE)imm_i;                                                   \
    int32_t result = (int32_t)(val1 OP val2);                                  \
    if (pos) {                                                                 \
      std::string s = render_itype_alu(insn, #NAME, imm_i);                    \
      *pos << std::setw(instruction_width) << std::setfill(' ') << std::left   \
           << s;                                                               \
      *pos << "// " << render_reg(rd) << " = " << to_hex0x32(val1)             \
           << " " #OP " " << to_hex0x32(val2) << " = " << to_hex0x32(result);  \
    }                                                                          \
    regs.set(rd, result);                                                      \
    pc += 4;                                                                   \
  }

#define ALU_SLT_IMM(NAME, OP, TYPE, LOG_OP)                                    \
  void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) {             \
    uint32_t rd = get_rd(insn);                                                \
    uint32_t rs1 = get_rs1(insn);                                              \
    int32_t imm_i = get_imm_i(insn);                                           \
    TYPE val1 = (TYPE)regs.get(rs1);                                           \
    TYPE val2 = (TYPE)imm_i;                                                   \
    int32_t result = (val1 OP val2) ? 1 : 0;                                   \
    if (pos) {                                                                 \
      std::string s = render_itype_alu(insn, #NAME, imm_i);                    \
      *pos << std::setw(instruction_width) << std::setfill(' ') << std::left   \
           << s;                                                               \
      *pos << "// " << render_reg(rd) << " = (" << to_hex0x32(val1)            \
           << " " LOG_OP " " << std::dec << val2                               \
           << ") ? 1 : 0 = " << to_hex0x32(result);                            \
    }                                                                          \
    regs.set(rd, result);                                                      \
    pc += 4;                                                                   \
  }

#define ALU_SHIFT_IMM(NAME, OP, TYPE)                                          \
  void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) {             \
    uint32_t rd = get_rd(insn);                                                \
    uint32_t rs1 = get_rs1(insn);                                              \
    int32_t imm_i = get_imm_i(insn);                                           \
    uint32_t shamt = imm_i & 0x1f;                                             \
    TYPE val1 = (TYPE)regs.get(rs1);                                           \
    int32_t result = (int32_t)(val1 OP shamt);                                 \
    if (pos) {                                                                 \
      std::string s = render_itype_alu(insn, #NAME, shamt);                    \
      *pos << std::setw(instruction_width) << std::setfill(' ') << std::left   \
           << s;                                                               \
      *pos << "// " << render_reg(rd) << " = " << to_hex0x32(val1)             \
           << " " #OP " " << std::dec << shamt << " = " << to_hex0x32(result); \
    }                                                                          \
    regs.set(rd, result);                                                      \
    pc += 4;                                                                   \
  }

ALU_IMM(addi, +, int32_t)
ALU_IMM(andi, &, int32_t)
ALU_IMM(ori, |, int32_t)
ALU_IMM(xori, ^, int32_t)

ALU_SLT_IMM(slti, <, int32_t, "<")
ALU_SLT_IMM(sltiu, <, uint32_t, "<U")

ALU_SHIFT_IMM(slli, <<, uint32_t)
ALU_SHIFT_IMM(srli, >>, uint32_t)
ALU_SHIFT_IMM(srai, >>, int32_t)

#undef ALU_IMM
#undef ALU_SLT_IMM
#undef ALU_SHIFT_IMM

#define LOAD_OP(NAME, MEM_FUNC, LOG_OP)                                        \
  void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) {             \
    uint32_t rd = get_rd(insn);                                                \
    uint32_t rs1 = get_rs1(insn);                                              \
    int32_t imm_i = get_imm_i(insn);                                           \
    uint32_t addr = regs.get(rs1) + imm_i;                                     \
    int32_t val = mem.MEM_FUNC(addr);                                          \
    if (pos) {                                                                 \
      std::string s = render_itype_load(insn, #NAME);                          \
      *pos << std::setw(instruction_width) << std::setfill(' ') << std::left   \
           << s;                                                               \
      *pos << "// " << render_reg(rd) << " = " LOG_OP "(m" << std::dec         \
           << (sizeof(val) *                                                   \
               8) /* Approximate bit width logic or hardcode strings */        \
           << "(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm_i)   \
           << ")) = " << to_hex0x32(val);                                      \
    }                                                                          \
    regs.set(rd, val);                                                         \
    pc += 4;                                                                   \
  }

LOAD_OP(lb, get8_sx, "sx");
LOAD_OP(lh, get16_sx, "sx");
LOAD_OP(lw, get32_sx, "sx");
LOAD_OP(lbu, get8, "zx");
LOAD_OP(lhu, get16, "zx");

#undef LOAD_OP

#define B_TYPE_IMPL(NAME, OP, TYPE, MNEMONIC, LOG_OP)                          \
  void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) {             \
    uint32_t rs1 = get_rs1(insn);                                              \
    uint32_t rs2 = get_rs2(insn);                                              \
    int32_t imm_b = get_imm_b(insn);                                           \
    TYPE val1 = (TYPE)regs.get(rs1);                                           \
    TYPE val2 = (TYPE)regs.get(rs2);                                           \
    bool take = (val1 OP val2);                                                \
    int32_t offset = take ? imm_b : 4;                                         \
    if (pos) {                                                                 \
      std::string s = render_btype(pc, insn, MNEMONIC);                        \
      *pos << std::setw(instruction_width) << std::setfill(' ') << std::left   \
           << s;                                                               \
      *pos << "// pc += (" << to_hex0x32(val1) << " " LOG_OP " "               \
           << to_hex0x32(val2) << " ? " << to_hex0x32(imm_b)                   \
           << " : 4) = " << to_hex0x32(pc + offset);                           \
    }                                                                          \
    pc += offset;                                                              \
  }

B_TYPE_IMPL(beq, ==, int32_t, "beq", "==")
B_TYPE_IMPL(bne, !=, int32_t, "bne", "!=")
B_TYPE_IMPL(blt, <, int32_t, "blt", "<")
B_TYPE_IMPL(bge, >=, int32_t, "bge", ">=")

B_TYPE_IMPL(bltu, <, uint32_t, "bltu", "<U")
B_TYPE_IMPL(bgeu, >=, uint32_t, "bgeu", ">=U")

#undef B_TYPE_IMPL

#define STORE_OP(NAME, MEM_FUNC, M_TYPE)                                       \
  void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) {             \
    uint32_t rs1 = get_rs1(insn);                                              \
    uint32_t rs2 = get_rs2(insn);                                              \
    int32_t imm_s = get_imm_s(insn);                                           \
    uint32_t addr = regs.get(rs1) + imm_s;                                     \
    mem.MEM_FUNC(addr, regs.get(rs2));                                         \
    if (pos) {                                                                 \
      std::string s = render_stype(insn, #NAME);                               \
      *pos << std::setw(instruction_width) << std::setfill(' ') << std::left   \
           << s;                                                               \
      uint32_t val = regs.get(rs2);                                            \
      if (std::string(#NAME) == "sb")                                          \
        val &= 0xff;                                                           \
      else if (std::string(#NAME) == "sh")                                     \
        val &= 0xffff;                                                         \
      *pos << "// " M_TYPE "(" << to_hex0x32(regs.get(rs1)) << " + "           \
           << to_hex0x32(imm_s) << ") = " << to_hex0x32(val);                  \
    }                                                                          \
    pc += 4;                                                                   \
  }

// Generate the functions
STORE_OP(sb, set8, "m8")
STORE_OP(sh, set16, "m16")
STORE_OP(sw, set32, "m32")

#undef STORE_OP
