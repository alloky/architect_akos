#include "stack.h"


class CpuEmu {
	long long regs[132];
	/*
	 *	rps[0] : tmp regiter
	 *	rps[1] : tmp2 regiter
	 *	rps[2] : flag register
	 *
	 *
	 *
	*/
	long long rps[10];

	stack<long long> st{};
	long long* heap{nullptr};
	long long* data{nullptr};

	size_t DATA_SIZE;	

public:

	CpuEmu(size_t max_ds);
	
	~CpuEmu();

	void execute(const char* path);

private:
	char* code;
	char* intsruction_pointer;

	void exec_loop();
	void process_instruction();
	
	/*
	 *	Arithmetic instructions
	 *	
	*/
	void add();
	void sub();
	void mul();
	void div();
	void mod();

	/*
	 *	Compare top of stack with next on stack
	*/
	void cmp();
	
	void mov(size_t r_ind);

	void jmp(size_t addr);
	void jl(size_t addr);
	void jle(size_t addr);
	void jeq(size_t addr);
	void jg(size_t addr);
	void jge(size_t addr);
	
	void push(long long val);
	void pop();

	void in();
	void out();

}


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
 *	12 - jq
*/