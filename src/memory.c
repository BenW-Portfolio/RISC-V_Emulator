#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h> 
#include <memory.h> 
#include <processor.h> 
#include <instructions.h>
#include <registers.h>

typedef unsigned long addr_t; // Address type matches config fields and register functions
typedef unsigned long reg_t;  // Register type matches register functions


struct memory {
    uint8_t *ram;         // Pointer to the byte array simulating RAM
    unsigned long size;   // Size of the allocated RAM block in bytes
};


/*
 * Allocate and free the memory struct. You can store whatever you
 * need in the memory structure (you define it yourself). The
 * `memory_sz` is the total amount of memory required in the program.
 */
struct memory *memory_alloc(unsigned long memory_sz) {
    struct memory *m = (struct memory *)malloc(sizeof(struct memory));
    if (m == NULL) {
        perror("Failed to allocate memory structure");
        return NULL;
    }

    m->size = memory_sz;
    m->ram = NULL; // Initialize ram pointer

    // Allocate the actual RAM byte array if size is non-zero
    if (memory_sz > 0) {
        m->ram = (uint8_t *)malloc(memory_sz);
        if (m->ram == NULL) {
            perror("Failed to allocate RAM block");
            free(m); // Clean up the structure allocation if RAM allocation fails
            return NULL;
        }
        // Initialize the allocated RAM to zero
        memset(m->ram, 0, memory_sz);
    }

    // fprintf(stderr, "DEBUG: Allocated memory struct %p, RAM block %p, size %lu\n", (void*)m, (void*)m->ram, m->size);
    return m;
}


void memory_free(struct memory *m) {
    if (m != NULL) {
        // fprintf(stderr, "DEBUG: Freeing RAM block %p (size %lu), memory struct %p\n", (void*)m->ram, m->size, (void*)m);
        if (m->ram != NULL) {
            free(m->ram); // Free the byte array first
        }
        free(m); // Then free the structure
    }
}

/* Get the current size of memory*/
unsigned long memory_getsize(struct memory *m) {
    if (m == NULL) {
        return 0;
    }
    return m->size;
}

/* Given an address, receive the byte at that memory location */
char memory_getbyte(struct memory *m, unsigned long addr) {
    // Check for valid memory structure, valid RAM pointer, and if address is within bounds
    if (m == NULL || m->ram == NULL || addr >= m->size) {
        return 0; // Return 0 for invalid access
    }
    return (char)m->ram[addr];
}


int processor_emulate_memops(struct processor *p, struct instruction *instr, enum exception *e) {
    // Validate Inputs & Check if it's a Memory Operation we handle
    if (!p || !p->rs || !p->mem || !instr || !e) {
        fprintf(stderr, "ERROR: Invalid NULL arguments passed to processor_emulate_memops.\n");
        if (e) *e = EXCEPTION_UNKNOWN_SYSCALL;
        return -1; // Indicate failure
    }

    int is_store = 0;
    size_t access_size = 0;     // Size of data access in bytes (1, 2, 4, 8)
    int is_signed_load = 0;     // Used for differentiating signed/unsigned loads
    reg_t current_pc = registers_getpc(p->rs); // Get PC for potential update
    int is_memory_op = 1; // Flag to track if it's an operation we need to call cache_access for

    // Determine operation type, access size, and load sign-extension requirement
    switch (instr->inst_no) {
        // Store Instructions
        case SB: access_size = 1; is_store = 1; break;
        case SH: access_size = 2; is_store = 1; break;
        case SW: access_size = 4; is_store = 1; break;
        case SD: access_size = 8; is_store = 1; break;
        // Load Instructions
        case LB: access_size = 1; is_signed_load = 1; break;
        case LH: access_size = 2; is_signed_load = 1; break;
        case LW: access_size = 4; is_signed_load = 1; break;
        case LD: access_size = 8; is_signed_load = 0; break;
        case LBU: access_size = 1; is_signed_load = 0; break;
        case LHU: access_size = 2; is_signed_load = 0; break;
        case LWU: access_size = 4; is_signed_load = 0; break;
        default:
            is_memory_op = 0; // Not a load/store we handle here
            return 0; // Success
    }

    // Check if RAM was actually allocated if we need to access it
    if (p->mem->ram == NULL) {
        if (p->mem->size == 0) {
            *e = EXCEPTION_MEMORY;
            return -1;
        } else {
            fprintf(stderr, "ERROR: Memory RAM pointer is NULL despite non-zero size (%lu).\n", p->mem->size);
            *e = EXCEPTION_MEMORY;
            return -1;
        }
    }


    // Verify Instruction Format & Extract Operands
    if (instr->fmt_type != FMT_ROR) {
        fprintf(stderr, "ERROR: Memory instruction %s has incorrect format type %d (expected FMT_ROR).\n",
                instr_name(instr->inst_no), instr->fmt_type);
        *e = EXCEPTION_INSTRUCTION;
        return -1;
    }

    unsigned int rs1_idx = instr->format.ror.r1;
    unsigned int r2_idx = instr->format.ror.r2;
    int64_t imm_offset = instr->format.ror.offset;

    // Calculate Effective Address
    reg_t base_addr = registers_getreg(p->rs, rs1_idx);
    addr_t effective_addr = base_addr + imm_offset;

    // Check Address Validity (Bounds Checking)
    addr_t min_valid_addr = p->cfg.instr_upper;
    addr_t max_valid_addr;

    int config_overflow = __builtin_add_overflow(p->cfg.instr_upper, p->cfg.memory_sz, &max_valid_addr);
    if (config_overflow) {
        max_valid_addr = (addr_t)-1;
    }

    addr_t access_end_addr;
    int access_overflow = __builtin_add_overflow(effective_addr, access_size, &access_end_addr);

    if (access_overflow || effective_addr < min_valid_addr || access_end_addr > max_valid_addr) {
        *e = EXCEPTION_MEMORY;
        return -1;
    }
    
    if (is_memory_op) { // Only call for actual load/store instructions
        cache_access(p->cache, effective_addr);
    }

    // Calculate Index into RAM Array
    size_t ram_index = effective_addr - min_valid_addr;
    assert(ram_index < p->mem->size && (ram_index + access_size) <= p->mem->size);

    // Perform Load or Store Operation
    void *mem_ptr = (void *)(p->mem->ram + ram_index);

    if (is_store) {
        unsigned int rs2_idx = r2_idx;
        reg_t value_to_store = registers_getreg(p->rs, rs2_idx);

        switch (access_size) {
            case 1: *(uint8_t *) mem_ptr = (uint8_t) value_to_store; break;
            case 2: *(uint16_t *)mem_ptr = (uint16_t)value_to_store; break;
            case 4: *(uint32_t *)mem_ptr = (uint32_t)value_to_store; break;
            case 8: *(uint64_t *)mem_ptr = (uint64_t)value_to_store; break;
        }
    } else { // Load operation
        unsigned int rd_idx = r2_idx;

        if (rd_idx == 0) {
            // Perform volatile read for potential side effects/checks but discard
            volatile uint64_t discard;
             switch (access_size) {
                 case 1: discard = *(uint8_t *)mem_ptr; break;
                 case 2: discard = *(uint16_t *)mem_ptr; break;
                 case 4: discard = *(uint32_t *)mem_ptr; break;
                 case 8: discard = *(uint64_t *)mem_ptr; break;
                 default: break;
             }
            (void)discard;
            // *** PC still needs to be advanced even for load to x0 ***
            registers_setpc(p->rs, current_pc + 4);
            return 0;
        }

        reg_t loaded_value = 0;

        switch (access_size) {
            case 1:
                loaded_value = is_signed_load ? (reg_t)(*(int8_t *)mem_ptr) : (reg_t)(*(uint8_t *)mem_ptr);
                break;
            case 2:
                loaded_value = is_signed_load ? (reg_t)(*(int16_t *)mem_ptr) : (reg_t)(*(uint16_t *)mem_ptr);
                break;
            case 4:
                loaded_value = is_signed_load ? (reg_t)(*(int32_t *)mem_ptr) : (reg_t)(*(uint32_t *)mem_ptr);
                break;
            case 8:
                 loaded_value = (reg_t)(*(uint64_t *)mem_ptr);
                 break;
        }
        registers_setreg(p->rs, rd_idx, loaded_value);
    }

    registers_setpc(p->rs, current_pc + 4);

    return 0; // Indicate successful execution
}