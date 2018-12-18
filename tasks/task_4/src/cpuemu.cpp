#pragma once

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
		LEV_LOG(LL_ERR, "data segment allocation failed with error:");
		LEV_LOG(LL_ERR, "Error : " << e.what());
	}
}

CpuEmu::~CpuEmu(){
	if( data != nullptr ) delete data;
}

void CpuEmu::IN(){
	std::cin >> rps[0];
	st.push(rps[0]);
	++instruction_pointer;
}

void CpuEmu::OUT() {
	rps[0] = st.pop();
	std::cout << rps[0] << std::endl;
	++instruction_pointer;
}

void CpuEmu::PUSH(){
	++instruction_pointer;
	st.push(extract_type<long long>());
}

void CpuEmu::POP(){
	st.pop();
	++instruction_pointer;
}

void CpuEmu::PUSHR() {
	++instruction_pointer;
	char r_ind = extract_type<char>();
	st.push(regs[r_ind]);
}

void CpuEmu::POPR() {
	++instruction_pointer;
	char r_ind = extract_type<char>();
	regs[r_ind] = st.pop();
}

void CpuEmu::MOV(){
	++instruction_pointer;
	char r_ind = extract_type<char>();
	long long val = extract_type<long long>();
	regs[r_ind] = val;
}

void CpuEmu::SQRT() {
	++instruction_pointer;
	char reg = extract_type<char>();
	regs[reg] = sqrt(regs[reg]);
}

void CpuEmu::CMP() {
	rps[0] = st.pop();
	rps[1] = st.pop();
	rps[2] = 0;
	if (rps[0] < rps[1]) {
		rps[2] = 2;
	}
	if (rps[0] > rps[1]) {
		rps[2] = 1;
	}
	st.push(rps[1]);
	st.push(rps[0]);
	++instruction_pointer;
}

void CpuEmu::JMP(){
	++instruction_pointer;
	size_t addr = extract_type<size_t>();
	instruction_pointer = code + addr;
}

void CpuEmu::JL(){
	++instruction_pointer;
	size_t addr = extract_type<size_t>();
	rps[0] = rps[2] & 3;
	if( rps[0] == 1 ){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::JLE(){
	++instruction_pointer;
	size_t addr = extract_type<size_t>();
	rps[0] = rps[2] & 3;
	if(rps[0] == 1 || rps[0] == 0){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::JEQ(){
	++instruction_pointer;
	size_t addr = extract_type<size_t>();
	rps[0] = rps[2] & 3;
	if(rps[0] == 0){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::JGE(){
	++instruction_pointer;
	size_t addr = extract_type<size_t>();
	rps[0] = rps[2] & 3;
	if(rps[0] == 2 || rps[0] == 0){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::JG(){
	++instruction_pointer;
	size_t addr = extract_type<size_t>();
	rps[0] = rps[2] & 3;
	if(rps[0] == 2){
		instruction_pointer = code + addr;
	}
}

void CpuEmu::SHOT() {
	int type = rand() % 2;
	if (type) {
		*((char*)&st + rand() % sizeof(st)) = rand() % 1000;
	}
	else {
		regs[rand() % 132] = rand() % 1000;
	}
	++instruction_pointer;
}

void CpuEmu::CALL() {
	ret_st.push(instruction_pointer + 1 + sizeof(size_t));
	JMP();
}

void CpuEmu::RET() {
	instruction_pointer = ret_st.pop();
}

#define ST_ARITHM_INST(cmd, op) \
void CpuEmu:: ## cmd ## (){ \
	++instruction_pointer; \
	rps[0] = st.pop(); \
	rps[1] = st.pop(); \
	rps[0] = rps[0] op rps[1]; \
	st.push(rps[0]); \
}

ST_ARITHM_INST(ADD, +)
ST_ARITHM_INST(SUB, -)
ST_ARITHM_INST(MUL, *)
ST_ARITHM_INST(MOD, %)

#undef ST_ARITHM_INST

void CpuEmu::DIV(){
	++instruction_pointer;
	rps[0] = st.pop();
	rps[1] = st.pop();
	if (rps[1] == 0) {
		LEV_LOG(LL_ERR, "Zero divison occured on instruction " << instruction_pointer);
		throw std::exception("zero division occured");	
	}
	rps[0] = rps[0] / rps[1];
	st.push(rps[0]);
}

#define LEFTWISE_REG_INSTRUCTION(cmd, op) \
void CpuEmu:: ## cmd ## _RL() {\
	++instruction_pointer; \
	char reg_num = extract_type<char>(); \
	long long val = extract_type<long long>(); \
	regs[reg_num] op val; \
} \
void CpuEmu:: ## cmd ## _RR() {\
	++instruction_pointer; \
	char reg_num_l = extract_type<char>(); \
	char reg_num_r = extract_type<char>(); \
	regs[reg_num_l] op regs[reg_num_r]; \
} \
void CpuEmu:: ## cmd ## _SR() {	\
	++instruction_pointer; \
	char reg_num_l = extract_type<char>(); \
	char reg_num_r = extract_type<char>(); \
	data[regs[reg_num_l]] op regs[reg_num_r]; \
} \
void CpuEmu:: ## cmd ## _RS() {	\
	++instruction_pointer; \
	char reg_num_l = extract_type<char>(); \
	char reg_num_r = extract_type<char>(); \
	regs[reg_num_l] op data[regs[reg_num_r]]; \
} \
void CpuEmu:: ## cmd ## _SS() {	\
	++instruction_pointer; \
	char reg_num_l = extract_type<char>(); \
	char reg_num_r = extract_type<char>(); \
	data[regs[reg_num_l]] op data[regs[reg_num_r]]; \
}


LEFTWISE_REG_INSTRUCTION(ADDL, += )
LEFTWISE_REG_INSTRUCTION(SUBL, -= )
LEFTWISE_REG_INSTRUCTION(MULL, *= )
LEFTWISE_REG_INSTRUCTION(MODL, %= )
LEFTWISE_REG_INSTRUCTION(MOVL, =  )

#undef RR_LL_REG_INSTRUCTION

void CpuEmu::DIVL_RL() {
	++instruction_pointer;
	char reg_num_l = extract_type<char>();
	long long val = extract_type<long long>();
	if (val == 0) {
		LEV_LOG(LL_ERR, "Zero divison occured on instruction " << instruction_pointer);
		throw std::exception("zero division occured");
	}
	regs[reg_num_l] /= val;
}

void CpuEmu::DIVL_RR() {
	++instruction_pointer;
	char reg_num_l = extract_type<char>();
	char reg_num_r = extract_type<char>();
	if (regs[reg_num_r] == 0) {
		LEV_LOG(LL_ERR, "Zero divison occured on instruction " << instruction_pointer);
		throw std::exception("zero division occured");
	}
	regs[reg_num_l] /= regs[reg_num_r];
}

void CpuEmu::DIVL_RS() {
	++instruction_pointer;
	char reg_num_l = extract_type<char>();
	char reg_num_r = extract_type<char>();
	if (regs[reg_num_r] == 0) {
		LEV_LOG(LL_ERR, "Zero divison occured on instruction " << instruction_pointer);
		throw std::exception("zero division occured");
	}
	regs[reg_num_l] /= data[regs[reg_num_r]];
}

void CpuEmu::DIVL_SR() {
	++instruction_pointer;
	char reg_num_l = extract_type<char>();
	char reg_num_r = extract_type<char>();
	if (regs[reg_num_r] == 0) {
		LEV_LOG(LL_ERR, "Zero divison occured on instruction " << instruction_pointer);
		throw std::exception("zero division occured");
	}
	data[regs[reg_num_l]] /= regs[reg_num_r];
}


void CpuEmu::DIVL_SS() {
	char reg_num_l = extract_type<char>();
	char reg_num_r = extract_type<char>();
	if (regs[reg_num_r] == 0) {
		LEV_LOG(LL_ERR, "Zero divison occured on instruction " << instruction_pointer);
		throw std::exception("zero division occured");
	}
	data[regs[reg_num_l]] /= data[regs[reg_num_r]];
}


void CpuEmu::execute(const char* path) {
	CodeLoader cl;
	code = cl.load_binary(path);
	instruction_pointer = code;
	code_size = cl.size;
	byte_code = std::string(code, code_size);
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
		// Codegeneration for cmd processing
#define DEF_CMD(CMD, WORD, COND, PARSE_CODE) \
	case CPUE_CMD_NUM:: ## CMD : \
		if(true) { CMD(); } \
		break;
#include "cpuemu-cmd-defs.h"
#undef DEF_CMD
		default:
			LEV_LOG(LL_ERR, "Unknown command found on addr");
			break;
	}
}

template <class T>
T CpuEmu::extract_type() {
	T ret = *((T*)instruction_pointer);
	instruction_pointer += sizeof(T);
	return ret;
}


#include <windows.h>

void CpuEmu::MEW() {
	++instruction_pointer;
	const std::string cat =

		"                   ;,_            ,                                    \n"
		"                  _uP~'b          d'u,                                 \n"
		"                 dP'   ''b       ,d'  'o                               \n"
		"                d'    , `b     d''    'b                               \n"
		"               l] [    ' `l,  d'       lb                              \n"
		"               Ol ?     '  'b`'=uoqo,_  'l                             \n"
		"             ,dBb 'b        'b,    `'~~TObup,_                         \n"
		"           ,d' (db.`'         ''     'tbc,_ `~'Yuu,_                   \n"
		"         .d' l`T'  '=                      ~     `''Yu,                \n"
		"       ,dO` gP,                           `u,   b,_  'b7               \n"
		"      d?' ,d' l,                           `'b,_ `~b  '1               \n"
		"    ,8i' dl   `l                 ,ggQOV',dbgq,._'  `l  lb              \n"
		"   .df' (O,    '             ,ggQY'~  , @@@@@d'bd~  `b '1              \n"
		"  .df'   `'           -=@QgpOY''     (b  @@@@P db    `Lp'b,            \n"
		" .d(                  _               'ko '=d_,Q`  ,_  '  'b,          \n"
		" Ql         .         `'qo,._          'tQo,_`''bo ;tb,    `'b,        \n"
		" qQ         |L           ~'QQQgggc,_.,dObc,opooO  `'~~';.   __,7,      \n"
		" qp         t\io,_           `~'TOOggQV''''        _,dg,_ =PIQHib.     \n"
		" `qp        `Q['tQQQo,_                          ,pl{QOP''   7AFR`     \n"
		"   `         `tb  '''tQQQg,_             p' 'b   `       .;-.`Vl'      \n"
		"              'Yb      `'tQOOo,__    _,edb    ` .__   /`/'|  |b;=;.__  \n"
		"                            `'tQQQOOOOP''`'\QV;qQObob'`-._`\_~~-._     \n"
		"                                 ''''    ._        /   | |oP'\_   ~\   \n"
		"                                         `~'\ic,qggddOOP'|  |  ~\   `  \n"
		"                                           ,qP`'''|'   | `\ `;   `\    \n"
		"                                _        _,p'     |    |   `\`;    |   \n"
		"        Meooow!                 'boo,._dP'       `\     `\    `\       \n"
		"                                  `'7tY~'                              \n"
		"                                                                       \n";
	std::cout << std::endl;
	std::cout << cat;
	std::cout << std::endl;
	PlaySound(TEXT("meow.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
