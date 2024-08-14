#include "interpreter.h"
#include "machine.h"
#include "commands.h"

int exec_add(interpreter interp);
int exec_addi(interpreter interp);
int exec_addiu(interpreter interp);
int exec_addu(interpreter interp);
int exec_and(interpreter interp);
int exec_andi(interpreter interp);
int exec_beq(interpreter interp, instruct_def *inst);
int exec_bgez(interpreter interp, instruct_def *inst);
int exec_bgtz(interpreter interp, instruct_def *inst);
int exec_blez(interpreter interp, instruct_def *inst);
int exec_bltz(interpreter interp, instruct_def *inst);
int exec_bne(interpreter interp, instruct_def *inst);
int exec_break();
int exec_div(interpreter interp);
int exec_j(interpreter interp, instruct_def *inst);
int exec_jal(interpreter interp, instruct_def *inst);
int exec_jalr(interpreter interp, instruct_def *inst);
int exec_jr(interpreter interp, instruct_def *inst);
int exec_lb(interpreter interp);
int exec_lbu(interpreter interp);
int exec_lui(interpreter interp);
int exec_lw(interpreter interp);
int exec_mfhi(interpreter interp);
int exec_mflo(interpreter interp);
int exec_mult(interpreter interp);
int exec_nop();
int exec_or(interpreter interp);
int exec_ori(interpreter interp);
int exec_sb(interpreter interp);
int exec_seb(interpreter interp);
int exec_sll(interpreter interp);
int exec_slt(interpreter interp);
int exec_slti(interpreter interp);
int exec_sltiu(interpreter interp);
int exec_sltu(interpreter interp);
int exec_sra(interpreter interp);
int exec_srl(interpreter interp);
int exec_sub(interpreter interp);
int exec_subu(interpreter interp);
int exec_sw(interpreter interp);
int exec_syscall(interpreter interp);
int exec_xor(interpreter interp);
int exec_ext(interpreter interp);