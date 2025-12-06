/* Ethan Silo
	z1838047
	CSCI 463-PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#pragma once
#include "hex.h"

/**
 * @class rv32i_decode
 * @brief A class for decoding and rendering RISC-V 32-bit integer instructions.
 *
 * This class provides static methods to decode binary instruction words into
 * human-readable assembly strings. It handles various instruction formats
 * (R, I, S, B, U, J) and extracts immediate values and register indices.
 * It inherits from the hex class for hexadecimal formatting utilities.
 ********************************************************************************/
class rv32i_decode : public hex
{
public:

    /**
     * @brief Decodes a 32-bit instruction into its assembly representation.
     * @param addr The memory address where the instruction is located (used for PC-relative calculations).
     * @param insn The 32-bit instruction word to decode.
     * @return A std::string containing the disassembled instruction.
     ****************************************************************************/
    static std::string decode(uint32_t addr, uint32_t insn);

protected:
    static constexpr int mnemonic_width             = 8;

    static constexpr uint32_t opcode_lui            = 0b0110111; //0x37
    static constexpr uint32_t opcode_auipc          = 0b0010111; //0x17
    static constexpr uint32_t opcode_jal            = 0b1101111; //0x6f
    static constexpr uint32_t opcode_jalr           = 0b1100111; //0x67
    static constexpr uint32_t opcode_btype          = 0b1100011; //0x63
    static constexpr uint32_t opcode_load_imm       = 0b0000011; //0x03
    static constexpr uint32_t opcode_stype          = 0b0100011; //0x23
    static constexpr uint32_t opcode_alu_imm        = 0b0010011; //0x13
    static constexpr uint32_t opcode_rtype          = 0b0110011; //0x33
    static constexpr uint32_t opcode_system         = 0b1110011; //0x73

    static constexpr uint32_t funct3_beq            = 0b000;
    static constexpr uint32_t funct3_bne            = 0b001;
    static constexpr uint32_t funct3_blt            = 0b100;
    static constexpr uint32_t funct3_bge            = 0b101;
    static constexpr uint32_t funct3_bltu           = 0b110;
    static constexpr uint32_t funct3_bgeu           = 0b111;

    static constexpr uint32_t funct3_lb             = 0b000;
    static constexpr uint32_t funct3_lh             = 0b001;
    static constexpr uint32_t funct3_lw             = 0b010;
    static constexpr uint32_t funct3_lbu            = 0b100;
    static constexpr uint32_t funct3_lhu            = 0b101;

    static constexpr uint32_t funct3_sb             = 0b000;
    static constexpr uint32_t funct3_sh             = 0b001;
    static constexpr uint32_t funct3_sw             = 0b010;

    static constexpr uint32_t funct3_add            = 0b000;
    static constexpr uint32_t funct3_sll            = 0b001;
    static constexpr uint32_t funct3_slt            = 0b010;
    static constexpr uint32_t funct3_sltu           = 0b011;
    static constexpr uint32_t funct3_xor            = 0b100;
    static constexpr uint32_t funct3_srx            = 0b101;
    static constexpr uint32_t funct3_or             = 0b110;
    static constexpr uint32_t funct3_and            = 0b111;

    static constexpr uint32_t funct7_srl            = 0b0000000;
    static constexpr uint32_t funct7_sra            = 0b0100000;

    static constexpr uint32_t funct7_add            = 0b0000000;
    static constexpr uint32_t funct7_sub            = 0b0100000;

    static constexpr uint32_t insn_ecall            = 0x00000073;
    static constexpr uint32_t insn_ebreak           = 0x00100073;

    static constexpr uint32_t funct3_csrrw          = 0b001;
    static constexpr uint32_t funct3_csrrs          = 0b010;
    static constexpr uint32_t funct3_csrrc          = 0b011;
    static constexpr uint32_t funct3_csrrwi         = 0b101;
    static constexpr uint32_t funct3_csrrsi         = 0b110;
    static constexpr uint32_t funct3_csrrci         = 0b111;

    /**
     * @brief Extracts the opcode field (bits 0-6).
     * @param insn The instruction.
     * @return The opcode value.
     ****************************************************************************/
    static uint32_t get_opcode(uint32_t insn);

    /**
     * @brief Extracts the destination register (rd) field (bits 7-11).
     * @param insn The instruction.
     * @return The register index.
     ****************************************************************************/
    static uint32_t get_rd(uint32_t insn);

    /**
     * @brief Extracts the funct3 field (bits 12-14).
     * @param insn The instruction.
     * @return The funct3 value.
     ****************************************************************************/
    static uint32_t get_funct3(uint32_t insn);

    /**
     * @brief Extracts the source register 1 (rs1) field (bits 15-19).
     * @param insn The instruction.
     * @return The register index.
     ****************************************************************************/
    static uint32_t get_rs1(uint32_t insn);

    /**
     * @brief Extracts the source register 2 (rs2) field (bits 20-24).
     * @param insn The instruction.
     * @return The register index.
     ****************************************************************************/
    static uint32_t get_rs2(uint32_t insn);

    /**
     * @brief Extracts the funct7 field (bits 25-31).
     * @param insn The instruction.
     * @return The funct7 value.
     ****************************************************************************/
    static uint32_t get_funct7(uint32_t insn);

    /**
     * @brief Extracts the I-type immediate (bits 20-31).
     * @param insn The instruction.
     * @return The sign-extended immediate value.
     ****************************************************************************/
    static int32_t get_imm_i(uint32_t insn);

    /**
     * @brief Extracts the U-type immediate (bits 12-31).
     * @param insn The instruction.
     * @return The immediate value shifted into the upper 20 bits.
     ****************************************************************************/
    static int32_t get_imm_u(uint32_t insn);

    /**
     * @brief Extracts the B-type immediate (bits 31, 7, 25-30, 8-11).
     * @param insn The instruction.
     * @return The sign-extended immediate value.
     ****************************************************************************/
    static int32_t get_imm_b(uint32_t insn);

    /**
     * @brief Extracts the S-type immediate (bits 25-31, 7-11).
     * @param insn The instruction.
     * @return The sign-extended immediate value.
     ****************************************************************************/
    static int32_t get_imm_s(uint32_t insn);

    /**
     * @brief Extracts the J-type immediate (bits 31, 12-19, 20, 21-30).
     * @param insn The instruction.
     * @return The sign-extended immediate value.
     ****************************************************************************/
    static int32_t get_imm_j(uint32_t insn);

    static constexpr uint32_t XLEN = 32;

    /**
     * @brief Renders an error message for unknown instructions.
     * @return The error string.
     ****************************************************************************/
    static std::string render_illegal_insn();

    /**
     * @brief Renders the LUI instruction.
     * @param insn The instruction.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_lui(uint32_t insn);

    /**
     * @brief Renders the AUIPC instruction.
     * @param insn The instruction.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_auipc(uint32_t insn);

    /**
     * @brief Renders the JAL instruction.
     * @param addr The address of the instruction.
     * @param insn The instruction.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_jal(uint32_t addr, uint32_t insn);

    /**
     * @brief Renders the JALR instruction.
     * @param insn The instruction.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_jalr(uint32_t insn);

    /**
     * @brief Renders Branch type instructions (BEQ, BNE, etc.).
     * @param addr The address of the instruction.
     * @param insn The instruction.
     * @param mnemonic The instruction mnemonic (e.g., "beq").
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_btype(uint32_t addr, uint32_t insn, const char *mnemonic);

    /**
     * @brief Renders Load instructions (LB, LH, LW, etc.).
     * @param insn The instruction.
     * @param mnemonic The instruction mnemonic.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_itype_load(uint32_t insn, const char *mnemonic);

    /**
     * @brief Renders Store instructions (SB, SH, SW).
     * @param insn The instruction.
     * @param mnemonic The instruction mnemonic.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_stype(uint32_t insn, const char *mnemonic);

    /**
     * @brief Renders ALU immediate instructions (ADDI, ANDI, etc.).
     * @param insn The instruction.
     * @param mnemonic The instruction mnemonic.
     * @param imm_i The immediate value.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i);

    /**
     * @brief Renders Register-Register instructions (ADD, SUB, etc.).
     * @param insn The instruction.
     * @param mnemonic The instruction mnemonic.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_rtype(uint32_t insn, const char *mnemonic);

    /**
     * @brief Renders the ECALL instruction.
     * @return The string "ecall".
     ****************************************************************************/
    static std::string render_ecall();

    /**
     * @brief Renders the EBREAK instruction.
     * @return The string "ebreak".
     ****************************************************************************/
    static std::string render_ebreak();

    /**
     * @brief Renders CSR instructions using immediate values (CSRRW, CSRRS, etc.).
     * @param insn The instruction.
     * @param mnemonic The instruction mnemonic.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_csrrx(uint32_t insn, const char *mnemonic);

    /**
     * @brief Renders CSR instructions using immediate operands (CSRRWI, etc.).
     * @param insn The instruction.
     * @param mnemonic The instruction mnemonic.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_csrrxi(uint32_t insn, const char *mnemonic);

    /**
     * @brief Helper to format a register name (e.g., "x1").
     * @param r The register index.
     * @return The string "x" followed by the index.
     ****************************************************************************/
    static std::string render_reg(int r);

    /**
     * @brief Helper to format base/displacement addressing (e.g., "offset(base)").
     * @param base The base register index.
     * @param disp The displacement/offset.
     * @return The formatted string.
     ****************************************************************************/
    static std::string render_base_disp(uint32_t base, int32_t disp);

    /**
     * @brief Helper to format the instruction mnemonic.
     * @param m The mnemonic string.
     * @return The mnemonic formatted with padding.
     ****************************************************************************/
    static std::string render_mnemonic(const std::string &m);
};
