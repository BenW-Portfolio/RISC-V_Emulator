/* 
 * Populate this with your HW2 code
 */

 /* Replace this file with your own code */
#include <processor.h>
#include <registers.h>
#include <stdlib.h>
#include <stdio.h>
#include <types.h>


/*
 * This is the main structure that stores the registers state. In this
 * homework, this focuses on the registers. You'll need to modify this
 * structure to track whatever you find necessary to be able to
 * complete the assignment.
 *
 * Do not change the name of this structure as we'll use it in our
 * code.
 */
struct registers {
    u64_t regs[32];
    u64_t pc;
};

/*
 * Allocate a new registers state. This should initialize the
 * registers assuming that the initial instruction to execute is the
 * first in the instruction array. This should not be called with
 * `num_instr < 1`. All registers should be initialized to `0`.
 *
 * - `@init_pc_value` - The value to initialize the `pc` to.
 * - `@return` - The new registers state.
 */
struct registers *registers_alloc(unsigned long init_pc_value) {
	struct registers *rs = malloc(sizeof(struct registers));
    if (rs == NULL) {
        return NULL;
    }
    for (int i = 0; i < 32; i++) {
        rs->regs[i] = 0;
    }
    rs->pc = init_pc_value;
    return rs;
}

/*
 * Free the previously allocated registers.
 *
 * - `@regs` - the registers to be freed.
 */
void registers_free(struct registers *regs) {
	free(regs);
}

/*
 * This function will dump out the state of a single register within a
 * registers state.
 *
 * - `@state` -the current state of all registers
 * - `@regno` - which register's value to return, `0 = x0`, through
 *   `31 = x31`. This value must be between 0 and 31.
 * - `@return` - The value at the register corresponding to regno
 */
unsigned long registers_getreg(struct registers *state, unsigned int regno) {
	if (regno > 31) {
        return 0; 
    }
    return state->regs[regno];
}

/*
 * This function will update the state of a single register to a given
 * value.
 *
 * - `@state` - Same as above.
 * - `@regno` - which register's value to return, `0 = x0`, through
 *   `31 = x31`. This value must be between 0 and 31.
 * - `@value` - the value to set the register to.
 */
void registers_setreg(struct registers *state, unsigned int regno, unsigned long value) {
	if (regno > 31) {
        return; // Or handle the error as appropriate
    }
    if(regno != 0) { //x0 should always be zero
        state->regs[regno] = value;
    }
}

/*
 * Similar to `registers_getreg`, but simply return the program
 * counter (`pc`).
 *
 * - `@state` - same as above.
 * - `@return` - the current program counter.
 */
unsigned int registers_getpc(struct registers *state) {
	return state->pc;
}

/*
 * Similar to `registers_setreg`, but simply initialize the program
 * counter (`pc`).
 *
 * - `@state` - same as above.
 */
void registers_setpc(struct registers *state, unsigned long value) {
	state->pc = value;
}


int
processor_emulate_regops(struct processor *processor, struct instruction *inst, enum exception *e)
{
	(void) processor;
	(void) inst;
	(void) e;

	/*
	 * TODO: Given an instruction instr, emulate the instruction by performing operations on the processor 
	 * corresponding to the instruction. The instruction is guaranteed to be a register operation.
	 * You will need to:
	 * 1. Get the instruction's opcode and operands
	 * 2. Perform the operation on the processor's registers
	 * 3. Update the processor's pc
	 * 4. Return value in accordance with the function's docstring in processor.h
	 */

	if (!processor || !inst || !e) {
        return -1; // Invalid input
    }


    u64_t pc = registers_getpc(processor->rs);

	// Check if the instruction address is within valid bounds
    if (pc < processor->cfg.instr_lower || pc >= processor->cfg.instr_upper) {
        *e = EXCEPTION_INSTRUCTION;
        return -1;
    }

	switch (inst->inst_no) {
        case ADD: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, rs1_val + rs2_val);
            break;
        }
        case ADDI: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, rs1_val + (s64_t)inst->format.rri.imm);
            break;
        }
        case SUB: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, rs1_val - rs2_val);
            break;
        }
        case OR: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, rs1_val | rs2_val);
            break;
        }
        case ORI: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, rs1_val | (s64_t)inst->format.rri.imm);
            break;
        }
        case AND: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, rs1_val & rs2_val);
            break;
        }
        case ANDI: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, rs1_val & (s64_t)inst->format.rri.imm);
            break;
        }
        case XOR: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, rs1_val ^ rs2_val);
            break;
        }
        case XORI: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, rs1_val ^ (s64_t)inst->format.rri.imm);
            break;
        }
        case SLL: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, rs1_val << (rs2_val & 0x3F)); 
            break;
        }
        case SLLI: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, rs1_val << (inst->format.rri.imm & 0x3F));
            break;
        }
        case SRL: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, rs1_val >> (rs2_val & 0x3F));
            break;
        }
        case SRLI: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, rs1_val >> (inst->format.rri.imm & 0x3F));
            break;
        }
        case SRA: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, (s64_t)rs1_val >> (rs2_val & 0x3F)); // Arithmetic shift
            break;
        }
        case SRAI: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, (s64_t)rs1_val >> (inst->format.rri.imm & 0x3F));
            break;
        }
        case ADDW: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, (s32_t)rs1_val + (s32_t)rs2_val); // 32-bit result
            break;
        }
        case ADDIW: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, (s32_t)rs1_val + (s32_t)inst->format.rri.imm); // Cast imm to s32_t
            break;
        }
        case SUBW: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, (s32_t)rs1_val - (s32_t)rs2_val);
            break;
        }
        case SLLW: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, (s32_t)rs1_val << (rs2_val & 0x1F));  //Lower 5 bits.
            break;

        }
        case SLLIW: {
             u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
             registers_setreg(processor->rs, inst->format.rri.rd, (s32_t)rs1_val << (inst->format.rri.imm & 0x1F) );
             break;
        }
        case SRLW: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, (u32_t)rs1_val >> (rs2_val & 0x1F));
            break;
        }
        case SRLIW: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, (u32_t)rs1_val >> (inst->format.rri.imm & 0x1F));
            break;
        }
        case SRAW: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rrr.rs1);
            u64_t rs2_val = registers_getreg(processor->rs, inst->format.rrr.rs2);
            registers_setreg(processor->rs, inst->format.rrr.rd, (s32_t)rs1_val >> (rs2_val & 0x1F));
            break;
        }
        case SRAIW: {
            u64_t rs1_val = registers_getreg(processor->rs, inst->format.rri.rs1);
            registers_setreg(processor->rs, inst->format.rri.rd, (s32_t)rs1_val >> (inst->format.rri.imm & 0x1F));
            break;
        }
        case LUI:{
            registers_setreg(processor->rs, inst->format.ri.rd, (inst->format.ri.imm << 12) ); 
            break;
        }
        case ECALL: {
            u64_t syscall_num = registers_getreg(processor->rs, 17);
            if (syscall_num == 23) {
                if (processor->cfg.print_regs) { 
                    processor_registers_printall(processor);
                }
                registers_setpc(processor->rs, pc + 4);
                return 1; 
            } else if (syscall_num == 2) {
                *e = EXCEPTION_EXIT;
                return -1; 
            } else {
                *e = EXCEPTION_UNKNOWN_SYSCALL;
                return -1;
            }
            break;
        }


        default:
            return 1; 
    }

    registers_setpc(processor->rs, pc + 4);  //Always increment pc by 4

    return 0; 

}

