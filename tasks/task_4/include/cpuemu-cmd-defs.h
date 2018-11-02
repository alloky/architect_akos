// Commands definition

DEF_CMD(ADD, "add",  {}, { add(); ++instruction_pointer; })
DEF_CMD(SUB, "sub",  {}, { sub(); ++instruction_pointer; })
DEF_CMD(MUL, "mul",  {}, { mul(); ++instruction_pointer; })
DEF_CMD(DIV, "div",  {}, { div(); ++instruction_pointer; })
DEF_CMD(MOD, "mod",  {}, { mod(); ++instruction_pointer; })
DEF_CMD(CMP, "cmp",  {}, { cmp(); ++instruction_pointer; })
DEF_CMD(MOV, "mov",  {

	PARSE_SIZE_T;	
	PARSE_LONG;
	
	}, { 
	++instruction_pointer;
	mov();
	instruction_pointer += sizeof(size_t) + sizeof(long long);
})
DEF_CMD(JMP, "jmp",  { PARSE_LABEL; }, {
	++instruction_pointer;
	jmp(*((size_t*)instruction_pointer)); 
})
DEF_CMD(JL, "jl",    { PARSE_LABEL; }, {
	++instruction_pointer;
	jl(*((size_t*)instruction_pointer)); 
})
DEF_CMD(JLE, "jle",  { PARSE_LABEL; }, {
	++instruction_pointer;
	jle(*((size_t*)instruction_pointer)); 
})
DEF_CMD(JEQ, "jeq",  { PARSE_LABEL; }, {
	++instruction_pointer;
	jeq(*((size_t*)instruction_pointer)); 
})
DEF_CMD(JGE, "jqe",  { PARSE_LABEL; }, {
	++instruction_pointer;
	jge(*((size_t*)instruction_pointer)); 
})
DEF_CMD(JG,  "jg",   { PARSE_LABEL; }, {
	++instruction_pointer;
	jg(*((size_t*)instruction_pointer)); 
})
DEF_CMD(PUSH, "push", { PARSE_LONG;  }, {
	++instruction_pointer;
	push(*((long long*)instruction_pointer));
	instruction_pointer += sizeof(long long); 
})
DEF_CMD(POP,  "pop",  {}, { pop(); ++instruction_pointer; })
DEF_CMD(IN,   "in",   {}, { in();  ++instruction_pointer; })
DEF_CMD(OUT,  "out",  {}, { out(); ++instruction_pointer; })
DEF_CMD(ADDL, "addl", {
	
}, {})