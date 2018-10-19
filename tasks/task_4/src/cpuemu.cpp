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

void CpuEmu::add(size_t addr);

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
	rps[0] = rps[0] / rps[1];
	st.push(rps[0]);
}


void CpuEmu::mod(){
	rps[0] = st.pop();
	rps[1] = st.pop();
	rps[0] = rps[0] % rps[1];
	st.push(rps[0]);
}


void CpuEmu::exec_loop(){
	while(true){
		bool flag = process_instruction(instruction_pointer);
		if(!flag){
			LEV_LOG(LL_INFO, "Execution ended.");
			break;
		}
	}
}

void CpuEmu::process_instruction(){
	
}
