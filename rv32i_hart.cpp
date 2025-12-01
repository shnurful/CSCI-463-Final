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
      exec_ecall(insn, pos);
    else if (insn == insn_ebreak)
      exec_ebreak(insn, pos);
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
        exec_illegal_insn(insn, pos);
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
      if (f7 == funct7_add)
        exec_add(insn, pos);
      else if (f7 == funct7_sub)
        exec_sub(insn, pos);
      else
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
        exec_illegal_insn(insn, pos);
        return;
      }
      assert(0 && "unrecognized funct7");
    }
    case funct3_xor: {
      exec_xor(insn, pos);
      return;
    }
    default: {
      exec_illegal_insn(insn, pos);
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
        exec_illegal_insn(insn, pos);
        return;
      }
      assert(0 && "unrecognized funct7 alu_srx");
    }
    case funct3_xor: {
      exec_xori(insn, pos);
      return;
    }
    default: {
      exec_illegal_insn(insn, pos);
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
      exec_illegal_insn(insn, pos);
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
      exec_illegal_insn(insn, pos);
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
      exec_illegal_insn(insn, pos);
      return;
      assert(0 && "unrecognized funct3 stype");
    }
    assert(0 && "stype fucked");
  }
  default: {
    exec_illegal_insn(insn, pos);
    return;
  }
  }
  assert(0 && "you fucked up. oopsies!");
}

void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream *pos) {
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
    std::cout << hdr << " " << to_hex0x32(pc) << " ";
    exec(insn, &std::cout);
  } else
    exec(insn, nullptr);
}

void rv32i_hart::exec_lui(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t imm_u = get_imm_u(insn);

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

  if (pos) {
    string s = render_auipc(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(pc) << " + "
         << to_hex0x32(imm_u) << ", "
         << "pc = (" << to_hex0x32(pc) << " + " << to_hex0x32(4) << ")";
  }

  regs.set(rd, (imm_u + pc));
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

  regs.set(rd, (pc + 4));
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
         << "pc = (" << to_hex0x32(pc) << " + " << to_hex0x32(4) << ") & "
         << to_hex0x32(0xfffffffe) << " = " << to_hex0x32(0);
  }

  regs.set(rd, (pc + 4));
  pc = r1 + imm_i;
}

void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream *pos){
  
  if(pos){
    string s = render_ebreak();
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s; 
    *pos << "// HALT";
  }
  halt = true;
  halt_reason = "EBREAK instruction";

}
void rv32i_hart::exec_ecall(uint32_t insn, std::ostream *pos){
  
  if(pos){
    string s = render_ecall();
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s; 
    *pos << "// HALT";
  }
  halt = true;
  halt_reason = "ECALL instruction";

}
#define CSR_OP(NAME, SOURCE, CALC_NEW) \
void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) { \
    uint32_t rd = get_rd(insn); \
    uint32_t rs1 = get_rs1(insn); \
    uint32_t csr_addr = get_imm_i(insn); \
    uint32_t source = SOURCE; \
    \
    uint32_t old_csr_val = 0; \
    if (csr_addr == 0xf14) { \
        old_csr_val = mhartid; \
    } else { \
      halt = true;\
      halt_reason = "Illegal CSR in CSRRS instruction";\
    } \
    \
    uint32_t new_csr_val = CALC_NEW; \
    \
    if (csr_addr == 0xf14) { \
    } \
    \
    if (pos) { \
        std::string s = render_csrrx(insn, #NAME); \
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s; \
        *pos << "// " << render_reg(rd) << " = " << to_hex0x32(old_csr_val); \
    } \
    regs.set(rd, old_csr_val); \
    pc += 4; \
}
CSR_OP(csrrw, regs.get(rs1), source)
CSR_OP(csrrs, regs.get(rs1), old_csr_val | source)
CSR_OP(csrrc, regs.get(rs1), old_csr_val & ~source)

CSR_OP(csrrwi, rs1, source)
CSR_OP(csrrsi, rs1, old_csr_val | source)
CSR_OP(csrrci, rs1, old_csr_val & ~source)

#undef CSR_OP


#define R_TYPE_ALU(NAME, OP, TYPE, MNEMONIC) \
void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) { \
    uint32_t rd = get_rd(insn); \
    uint32_t rs1 = get_rs1(insn); \
    uint32_t rs2 = get_rs2(insn); \
    TYPE val1 = (TYPE)regs.get(rs1); \
    TYPE val2 = (TYPE)regs.get(rs2); \
    int32_t result = (int32_t)(val1 OP val2); \
    if (pos) { \
        std::string s = render_rtype(insn, MNEMONIC); \
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s; \
        *pos << "// " << render_reg(rd) << " = " << to_hex0x32(val1) << " " #OP " " << to_hex0x32(val2) << " = " << to_hex0x32(result); \
    } \
    regs.set(rd, result); \
    pc += 4; \
}

#define R_TYPE_SHIFT(NAME, OP, TYPE, MNEMONIC) \
void rv32i_hart::exec_##NAME(uint32_t insn, std::ostream *pos) { \
    uint32_t rd = get_rd(insn); \
    uint32_t rs1 = get_rs1(insn); \
    uint32_t rs2 = get_rs2(insn); \
    TYPE val1 = (TYPE)regs.get(rs1); \
    uint32_t amount = regs.get(rs2) & 0x1f;\
    int32_t result = (int32_t)(val1 OP amount); \
    if (pos) { \
        std::string s = render_rtype(insn, MNEMONIC); \
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s; \
        *pos << "// " << render_reg(rd) << " = " << to_hex0x32(val1) << " " #OP " " << std::dec << amount << " = " << to_hex0x32(result); \
    } \
    regs.set(rd, result); \
    pc += 4; \
}


R_TYPE_ALU(add, +, int32_t, "add")
R_TYPE_ALU(sub, -, int32_t, "sub")
R_TYPE_ALU(and, &, int32_t, "and")
R_TYPE_ALU(or,  |, int32_t, "or")
R_TYPE_ALU(xor, ^, int32_t, "xor")

R_TYPE_ALU(slt,  <, int32_t,  "slt") 
R_TYPE_ALU(sltu, <, uint32_t, "sltu")

R_TYPE_SHIFT(sll, <<, uint32_t, "sll")
R_TYPE_SHIFT(srl, >>, uint32_t, "srl")
R_TYPE_SHIFT(sra, >>, int32_t,  "sra")

#undef R_TYPE_ALU
#undef R_TYPE_SHIFT
