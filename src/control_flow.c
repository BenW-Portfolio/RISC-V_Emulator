#include <stdio.h>
#include "processor.h"
#include "instructions.h"
#include "registers.h"
#include "types.h"

typedef u64_t addr_t;
typedef u64_t reg_t;

int processor_emulate_cntlflowops(struct processor *p, struct instruction *instr, enum exception *e) {
    // Validate Inputs
    if (!p || !p->rs || !instr || !e) {
        fprintf(stderr, "ERROR: Invalid NULL arguments passed to processor_emulate_cntlflowops.\n");
        if (e != NULL) {
            *e = EXCEPTION_UNKNOWN_SYSCALL;
        }
        return -1;
    }

    // Filter for instructions handled by this function
    switch (instr->inst_no) {
        case SLT: case SLTU: case SLTI: case SLTIU:
        case BEQ: case BNE: case BLT: case BGE: case BLTU: case BGEU:
        case JAL: case JALR: case AUIPC:
            break; // Continue execution within this function
        default:
            return 0; // Not handled in this function
    }

    // Instruction is handled here
    reg_t current_pc = registers_getpc(p->rs);
    reg_t next_pc = current_pc + 4; // Default next PC

    // Define variables for operands
    u64_t rs1_val_u = 0, rs2_val_u = 0;
    s64_t rs1_val_s = 0, rs2_val_s = 0;
    s64_t imm = 0;
    unsigned int rs1_idx = 0, rs2_idx = 0, rd_idx = 0; // Store indices for clarity

    // Fetch operands based on format AND instruction type
    switch (instr->fmt_type) {
        case FMT_RRR: // SLT, SLTU
            rd_idx = instr->format.rrr.rd;
            rs1_idx = instr->format.rrr.rs1;
            rs2_idx = instr->format.rrr.rs2;
            rs1_val_u = registers_getreg(p->rs, rs1_idx);
            rs2_val_u = registers_getreg(p->rs, rs2_idx);
            break;
        case FMT_RRI: // SLTI, SLTIU, and B* branches
            imm = instr->format.rri.imm;
            if (instr->inst_no >= BEQ && instr->inst_no <= BGEU) {
                 rs1_idx = instr->format.rri.rd; 
                 rs2_idx = instr->format.rri.rs1; 
                 rd_idx = 99; 
            } else {
                 // SLTI, SLTIU
                 rd_idx = instr->format.rri.rd;
                 rs1_idx = instr->format.rri.rs1;
                 rs2_idx = 99;
            }
            rs1_val_u = registers_getreg(p->rs, rs1_idx);

            if (rs2_idx != 99) {
                rs2_val_u = registers_getreg(p->rs, rs2_idx);
            }
            break;
        case FMT_RI: // AUIPC, JAL
            rd_idx = instr->format.ri.rd;
            imm = instr->format.ri.imm;
            rs1_idx = 99; rs2_idx = 99;
            break;
        case FMT_ROR: // JALR
            rd_idx = instr->format.ror.r2;
            rs1_idx = instr->format.ror.r1;
            imm = instr->format.ror.offset;
            rs1_val_u = registers_getreg(p->rs, rs1_idx);
            rs2_idx = 99;
            break;
        default:
            return -1;
    }

    // Cast to signed values after fetching unsigned
    rs1_val_s = (s64_t)rs1_val_u;
    rs2_val_s = (s64_t)rs2_val_u;

    // Execute Instruction Logic using fetched operands/indices
    switch (instr->inst_no) {
        // Set Instructions 
        case SLT:
            if (rd_idx != 0) {
                registers_setreg(p->rs, rd_idx, (rs1_val_s < rs2_val_s) ? 1 : 0);
            }
            break;
        case SLTU:
            if (rd_idx != 0) {
                registers_setreg(p->rs, rd_idx, (rs1_val_u < rs2_val_u) ? 1 : 0);
            }
            break;
        case SLTI:
            if (rd_idx != 0) {
                registers_setreg(p->rs, rd_idx, (rs1_val_s < imm) ? 1 : 0);
            }
            break;
        case SLTIU:
            if (rd_idx != 0) {
                registers_setreg(p->rs, rd_idx, (rs1_val_u < (u64_t)imm) ? 1 : 0);
            }
            break;

        // Branch Instructions
        case BEQ: 
            if (rs1_val_u == rs2_val_u) {
                next_pc = current_pc + imm;
            }
            break;
        case BNE: 
            if (rs1_val_u != rs2_val_u) {
                next_pc = current_pc + imm;
            }
            break;
        case BLT:
            if (rs1_val_s < rs2_val_s) {
                next_pc = current_pc + imm;
            }
            break;
        case BGE:
            if (rs1_val_s >= rs2_val_s) {
                next_pc = current_pc + imm;
            }
            break;
        case BLTU:
            if (rs1_val_u < rs2_val_u) {
                next_pc = current_pc + imm;
            }
            break;
        case BGEU: 
            if (rs1_val_u >= rs2_val_u) {
                next_pc = current_pc + imm;
            }
            break;

        // Jump Instructions
        case JAL: {
                if (rd_idx != 0) {
                    { registers_setreg(p->rs, rd_idx, current_pc + 4); }
                }
                next_pc = current_pc + imm;
            }
            break;
        case JALR: {
                addr_t target_base = rs1_val_u; 
                addr_t target_addr = (target_base + imm) & ~((addr_t)1);
                if (rd_idx != 0) { registers_setreg(p->rs, rd_idx, current_pc + 4); }
                next_pc = target_addr;
             }
            break;

        // PC-Relative Address Calculation
        case AUIPC: {
                if (rd_idx != 0) {
                    u64_t uimm_val = (u64_t)imm;
                    reg_t result = current_pc + (uimm_val << 12);
                    registers_setreg(p->rs, rd_idx, result);
                }
            }
             break;

        default: 
            break;
    }

    // Update Program Counter 
    registers_setpc(p->rs, next_pc);

    return 0; // Indicate successful execution
}