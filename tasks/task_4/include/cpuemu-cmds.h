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
 *	13 - PUSH
*	14 - POP
*	15 - IN
*	16 - OUT

*/

enum CPUE_CMD_NUM {
#define DEF_CMD(CMD, WORD, PARSE_CODE, PROC_CODE) CMD ,
#include "cpuemu-cmd-defs.h"
#undef DEF_CMD
};
