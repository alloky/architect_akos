#include "cpuemu.h"
#include "logging.h"
#include "CodeLoader.h"

CpuEmu::CpuEmu(size_t max_ds)
	: DATA_SIZE(max_ds)
{
	try {
		data = new long long[max_ds];
	} catch (const std::bad_alloc& e){
		LEV_LOG(LL_ERR, "CPU emulator init failed:");
		LEV_LOG(LL_ERR, "data segment allocation failed with error:");
		LEV_LOG(LL_ERR, "Error : " << e.what());
	}
}

CpuEmu::~CpuEmu(){
	if( data != nullptr ) delete data;
}

void CpuEmu::in(){
	std::cin >> rps[0];
	st.push(rps[0]);
}

void CpuEmu::out(){
	rps[0] = st.pop();
	std::cout << rps[0];
}

void CpuEmu::push(long long val){
	st.push(val);
}

void CpuEmu::pop(){
	st.pop();
}

void CpuEmu::mov(size_t r_ind){
	regs[r_ind] = st.pop();
}

void CpuEmu::cmp() {
	rps[0] = st.pop();
	rps[1] = st.pop();
	if (rps[0] <= rps[1]) {
		rps[2] += 2;
	}
	if (rps[0] >= rps[1]) {
		rps[2] += 1;
	}
	st.push(rps[1]);
	st.push(rps[0]);
}

void CpuEmu::jmp(size_t addr){
	instruction_pointer = code + addr;
}

void CpuEmu::jl(size_t addr){
	rps[0] = rps[2] & 3;
	if( rps[0] == 1 ){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::jle(size_t addr){
	rps[0] = rps[2] & 3;
	if(rps[0] == 1 || rps[0] == 3){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::jeq(size_t addr){
	rps[0] = rps[2] & 3;
	if(rps[0] == 3){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::jge(size_t addr){
	rps[0] = rps[2] & 3;
	if(rps[0] == 2 || rps[0] == 3){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::jg(size_t addr){
	rps[0] = rps[2] & 3;
	if(rps[0] == 2){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::add(){
	rps[0] = st.pop();
	rps[1] = st.pop();
	rps[0] = rps[0] + rps[1];
	st.push(rps[0]);
}


void CpuEmu::sub(){
	rps[0] = st.pop();
	rps[1] = st.pop();
	rps[0] = rps[0] - rps[1];
	st.push(rps[0]);
}


void CpuEmu::mul(){
	rps[0] = st.pop();
	rps[1] = st.pop();
	rps[0] = rps[0] * rps[1];
	st.push(rps[0]);
}


void CpuEmu::div(){
	rps[0] = st.pop();
	rps[1] = st.pop();
	if (rps[1] == 0) {
		LEV_LOG(LL_ERR, "Zero divison occured on instruction " << instruction_pointer);
		throw std::exception("zero division occured");	
	}
	rps[0] = rps[0] / rps[1];
	st.push(rps[0]);
}


void CpuEmu::mod(){
	rps[0] = st.pop();
	rps[1] = st.pop();
	rps[0] = rps[0] % rps[1];
	st.push(rps[0]);
}


void CpuEmu::execute(const char* path) {
	CodeLoader cl;
	code = cl.load_binary(path);
	instruction_pointer = code;
	code_size = cl.size;
	exec_loop();
}

void CpuEmu::exec_loop(){
	while(true){
		process_instruction();
		if (code + code_size <= instruction_pointer) break;
	}
	LEV_LOG(LL_INFO, "Execution ended.");
}

void CpuEmu::process_instruction(){
	switch (*instruction_pointer) {
		case CPUE_ADD_NUM:
			add();
			++instruction_pointer;
			break;
		case CPUE_SUB_NUM:
			sub();
			++instruction_pointer;
			break;
		case CPUE_MUL_NUM:
			mul();
			++instruction_pointer;
			break;
		case CPUE_DIV_NUM:
			div();
			++instruction_pointer;
			break;
		case CPUE_MOD_NUM:
			mod();
			++instruction_pointer;
			break;
		case CPUE_CMP_NUM:
			cmp();
			++instruction_pointer;
			break;
		case CPUE_MOV_NUM:
			++instruction_pointer;
			mov(*((size_t*)instruction_pointer));
			instruction_pointer += sizeof(size_t);
			break;
		case CPUE_JMP_NUM:
			++instruction_pointer;
			jmp(*((size_t*)instruction_pointer));
			break;
		case CPUE_JL_NUM:
			++instruction_pointer;
			jl(*((size_t*)instruction_pointer));
			break;
		case CPUE_JLE_NUM:
			++instruction_pointer;
			jle(*((size_t*)instruction_pointer));
			break;
		case CPUE_JEQ_NUM:
			++instruction_pointer;
			jeq(*((size_t*)instruction_pointer));
			break;
		case CPUE_JGE_NUM:
			++instruction_pointer;
			jge(*((size_t*)instruction_pointer));
			break;
		case CPUE_JG_NUM:
			++instruction_pointer;
			jg(*((size_t*)instruction_pointer));
			break;
		case CPUE_PUSH_NUM:
			++instruction_pointer;
			push(*((long long*)instruction_pointer));
			instruction_pointer += sizeof(long long);
			break;
		case CPUE_POP_NUM:
			pop();
			++instruction_pointer;
			break;
	}
}
