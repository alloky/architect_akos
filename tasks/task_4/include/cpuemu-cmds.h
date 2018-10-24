#pragma once

/*
 * Instruction mapping
 *
 * 	Arithmetics:
 *
 * 	0 - add
 *	1 - sub
 *	2 - mul
 *	3 - div
 *	4 - mod
 *
 *	5 - cmp
 *
 *	6 - mov
 *
 *	Jumps:
 *
 *	7 - jmp
 *	8 - jl
 *	9 - jle
 *	10 - jeq
 *	11 - jge
 *	12 - jg
*/

#define CPUE_ADD_NUM  0
#define CPUE_SUB_NUM  1
#define CPUE_MUL_NUM  2
#define CPUE_DIV_NUM  3
#define CPUE_MOD_NUM  4
#define CPUE_CMP_NUM  5
#define CPUE_MOV_NUM  6
#define CPUE_JMP_NUM  7
#define CPUE_JL_NUM   8
#define CPUE_JLE_NUM  9
#define CPUE_JEQ_NUM  10
#define CPUE_JGE_NUM  11
#define CPUE_JG_NUM   12
#define CPUE_PUSH_NUM 13
#define CPUE_POP_NUM  14

#define CPUE_ADD_WORD    "add"
#define CPUE_SUB_WORD    "sub"
#define CPUE_MUL_WORD    "mul"
#define CPUE_DIV_WORD    "div"
#define CPUE_MOD_WORD    "mod"
#define CPUE_CMP_WORD    "cmp"
#define CPUE_MOV_WORD    "mov"
#define CPUE_JMP_WORD    "jmp"
#define CPUE_JL_WORD     "jl"
#define CPUE_JLE_WORD    "jle"
#define CPUE_JEQ_WORD    "jeq"
#define CPUE_JGE_WORD    "jge"
#define CPUE_JG_WORD     "jg"
#define CPUE_PUSH_WORD   "push"
#define CPUE_POP_WORD    "add"