/* Replace this file with your own code */
/* Empty C file for instructions parsing.
 * Student can copy and paste their code here.
 * Meow
*/

/*
 * Copyright (c) Gabriel Parmer, 2024.
 * Do not distribute without permission.
 */

 #include <processor.h>
 #include <instructions.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 
 struct instructions *
 instructions_parse(char *prog, struct configuration *cfg)
 {
     (void)prog;
     (void)cfg;
 
     /* TODO: Parse the given program, create and populate a struct of instructions with the appropriate instructions types
      * and associated data. In addition, populate the configuration with the relevant information from the header of the program.
      */
 
     char **program_lines;
     int num_lines;
     int i;
     struct instructions *instrs = NULL;
 
 
     program_lines = lines(prog, &num_lines);
     if (program_lines == NULL || num_lines == 0) {
         return NULL;
     }
 
     // Parse the header line
     if (sscanf(program_lines[0], "0x%lx 0x%lx 0x%lx 0x%lx", &cfg->instr_lower, &cfg->instr_upper, &cfg->entry_address, &cfg->memory_sz) != 4) {
         free_lines(program_lines);
         return NULL;
     }
 
 
     // Allocate memory for the instructions struct
     instrs = malloc(sizeof(struct instructions));
     if (instrs == NULL) {
         free_lines(program_lines);
         return NULL;
     }
 
 
     instrs->num_instructions = num_lines - 1; // Subtract the header line
     instrs->instructions = malloc(sizeof(struct instruction) * instrs->num_instructions);
     if (instrs->instructions == NULL) {
         free(instrs);
         free_lines(program_lines);
         return NULL;
     }
 
     // Parse each instruction line
     for (i = 1; i < num_lines; i++) {
         char *line = program_lines[i];
         char instr_str[10]; 
         int rd, rs1, rs2;
         long imm, offset;
         struct instruction *current_instr = &instrs->instructions[i - 1];
 
         // Determine instruction type and parse accordingly
         if (sscanf(line, "%s", instr_str) != 1) {
             free(instrs->instructions);
             free(instrs);
             free_lines(program_lines);
             return NULL;
         }
 
         current_instr->inst_no = instr_num(instr_str);
         if (current_instr->inst_no == INSTRINVALID)
         {
             free(instrs->instructions);
             free(instrs);
             free_lines(program_lines);
             return NULL;
         }
         current_instr->fmt_type = instr_format(current_instr->inst_no);
         current_instr->inst_addr = cfg->instr_lower + (i -1) * 4;
 
         switch (current_instr->fmt_type) {
             case FMT_RRR:
                 if (sscanf(line, "%s x%d,x%d,x%d", instr_str, &rd, &rs1, &rs2) != 4) {
                     free(instrs->instructions);
                     free(instrs);
                     free_lines(program_lines);
                     return NULL;
                 }
                 if (rd < 0 || rd > 31 || rs1 < 0 || rs1 > 31 || rs2 < 0 || rs2 > 31) {
                    free(instrs->instructions);
                     free(instrs);
                     free_lines(program_lines);
                     return NULL;
                 }
                 current_instr->format.rrr.rd = rd;
                 current_instr->format.rrr.rs1 = rs1;
                 current_instr->format.rrr.rs2 = rs2;
                 break;
 
             case FMT_RRI:
                 if (sscanf(line, "%s x%d,x%d,%ld", instr_str, &rd, &rs1, &imm) == 4) { 
                     if (rd < 0 || rd > 31 || rs1 < 0 || rs1 > 31) {
                         free(instrs->instructions);
                         free(instrs);
                         free_lines(program_lines);
                         return NULL;
                     }
                     current_instr->format.rri.rd = rd;
                     current_instr->format.rri.rs1 = rs1;
                     current_instr->format.rri.imm = imm;
                 }
                 else if (sscanf(line, "%s x%d,x%d,%ld", instr_str, &rs1, &rs2, &imm) == 4){
                     if (rs1 < 0 || rs1 > 31 || rs2 < 0 || rs2 > 31){
                         free(instrs->instructions);
                         free(instrs);
                         free_lines(program_lines);
                         return NULL;
                     }
                     current_instr->format.rri.rd = rs1; 
                     current_instr->format.rri.rs1 = rs2; 
                     current_instr->format.rri.imm = imm;
                 }
 
                 else {
                    free(instrs->instructions);
                     free(instrs);
                     free_lines(program_lines);
                     return NULL;
                 }
                 break;
 
             case FMT_RI:
                 if (sscanf(line, "%s x%d,%ld", instr_str, &rd, &imm) != 3) {
                     free(instrs->instructions);
                     free(instrs);
                     free_lines(program_lines);
                     return NULL;
                 }
                 if (rd < 0 || rd > 31)
                 {
                    free(instrs->instructions);
                     free(instrs);
                     free_lines(program_lines);
                     return NULL;
                 }
                 current_instr->format.ri.rd = rd;
                 current_instr->format.ri.imm = imm;
                 break;
             case FMT_ROR:
                 if (current_instr->inst_no == JALR)
                 {
                     if(sscanf(line, "%s x%d, %ld(x%d)", instr_str, &rd, &offset, &rs1) != 4) 
                     {
                        free(instrs->instructions);
                         free(instrs);
                         free_lines(program_lines);
                         return NULL;
                     }
                     if (rs1 < 0 || rs1 > 31 || rd < 0 || rd >31)
                     {
                        free(instrs->instructions);
                         free(instrs);
                         free_lines(program_lines);
                         return NULL;
                     }
                     current_instr->format.ror.r1 = rs1;
                     current_instr->format.ror.r2 = rd; 
                     current_instr->format.ror.offset = offset;
 
                 }
                 else if (sscanf(line, "%s x%d,%ld(x%d)", instr_str, &rs2, &offset, &rs1) != 4) {
                     free(instrs->instructions);
                     free(instrs);
                     free_lines(program_lines);
                     return NULL;
                 }
                 else{
                      if (rs1 < 0 || rs1 > 31 || rs2 < 0 || rs2 >31)
                     {
                         free(instrs->instructions);
                         free(instrs);
                         free_lines(program_lines);
                         return NULL;
                     }
                     current_instr->format.ror.r1 = rs1;
                     current_instr->format.ror.r2 = rs2;
                     current_instr->format.ror.offset = offset;
                 }
                 break;
 
 
             case FMT_ECALL:
                 // No arguments to parse for ecall
                 break;
 
             default:
                free(instrs->instructions);
                 free(instrs);
                 free_lines(program_lines);
                 return NULL;
         }
     }
 
     free_lines(program_lines);
     return instrs;
 }
 
 void instructions_free(struct instructions *instrs)
 {
     if (instrs->instructions)
         free(instrs->instructions);
     free(instrs);
 }