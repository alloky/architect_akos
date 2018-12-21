#include "twitchLangCompiler.h"


std::string TwitchLangCompiler::compile(const std::string& _code, const std::string& dest) {
	code = _code;
	codeFlow.str(_code);
	active_node = &comptree.root;
	parseProgramm();
	compileToAsm(dest);
	return "";
}

/*

	ASSEBLING SECTION

*/


void TwitchLangCompiler::compileToAsm(const std::string& dest) {
	outfile.open(dest);
	if (outfile.good()) {
		scopes.push_back({});
		for (auto& el : is_reg_reserved) {
			el = false;
		}
		toAsm(comptree.root);
	}
	outfile << "\n";
	outfile << ":_THE_END_ \n";
	outfile << "\n";
	outfile.close();
}

void TwitchLangCompiler::toAsm(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node) {
	toAsmStatement(node);
}

void TwitchLangCompiler::toAsmStatement(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node) {
	if (!node) {
		return;
	}
	
	if (node->op == CompiledTree::NodeType::Declaration) {
		toAsmDeclareLocal(node->left.get()->value);
		toAsmStatement(node->right);
	}
	else if (node->op == CompiledTree::NodeType::Input) {
		int reg_num = findVarReg(node->left.get()->value);
		outfile << "in\n";
		outfile << "popr r" << reg_num << "\n";
		toAsmStatement(node->right);
	}
	else if (node->op == CompiledTree::NodeType::Output) {
		int reg_num = findVarReg(node->left.get()->value);
		outfile << "pushr r" << reg_num << "\n";
		outfile << "out\n";
		toAsmStatement(node->right);
	}
	else if (node->op == CompiledTree::NodeType::If) {
		label_count++;
		int reg_1 = findVarReg(node->left->left->value),
			reg_2 = findVarReg(node->left->right->value);
		outfile << "pushr r" << reg_1 << "\n";
		outfile << "pushr r" << reg_2 << "\n";
		outfile << "cmp\n";
		int else_lbl = label_count;
		switch (node->left.get()->op)
		{
			case CompiledTree::NodeType::Less:
				outfile << "jge :_else_" << label_count << "\n";
				break;
			case CompiledTree::NodeType::Equal:
				outfile << "jne :_else_" << label_count << "\n";
				break;
			case CompiledTree::NodeType::More:
				outfile << "jle :_else_" << label_count << "\n";
				break;
		}
		label_count++;
		scopes.push_back({});
		toAsmStatement(node->right->left);
		freeScope(scopes[scopes.size() - 1]);
		scopes.pop_back();
		
		int end_lbl = label_count;
		label_count++;
		outfile << "jmp :_if_end_" << end_lbl << "\n";
		
		outfile << ":_else_" << else_lbl << "\n";
		scopes.push_back({});
		toAsmStatement(node->right->right->left);
		freeScope(scopes[scopes.size() - 1]);
		scopes.pop_back();

		outfile << ":_if_end_" << end_lbl << "\n";
		toAsmStatement(node->right->right->right);
	}
	else if (node->op == CompiledTree::NodeType::While) {
		int reg_1 = findVarReg(node->left->left->value),
			reg_2 = findVarReg(node->left->right->value);
		int start_lbl = label_count;
		label_count++;
		outfile << ":_while_start_" << start_lbl << "\n";
		outfile << "pushr r" << reg_1 << "\n";
		outfile << "pushr r" << reg_2 << "\n";
		outfile << "cmp\n";
		int end_lbl = label_count;
		label_count++;
		switch (node->left.get()->op)
		{
		case CompiledTree::NodeType::Less:
			outfile << "jge :_while_end_" << end_lbl << "\n";
			break;
		case CompiledTree::NodeType::Equal:
			outfile << "jne :_while_end_" << end_lbl << "\n";
			break;
		case CompiledTree::NodeType::More:
			outfile << "jle :_while_	end_" << end_lbl << "\n";
			break;
		}
		
		scopes.push_back({});
		toAsmStatement(node->right->left);
		freeScope(scopes[scopes.size() - 1]);
		scopes.pop_back();

		outfile << "jmp :_while_start_" << start_lbl << "\n";
		outfile << ":_while_end_" << end_lbl << "\n";
		toAsmStatement(node->right->right);
	}
	else if (node->op == CompiledTree::NodeType::Assign) {
		int reg_num = findVarReg(node->left->right->value);
		calcExpression(node->left->left, 128);
		outfile << "movl r" << reg_num << " r128\n";
		toAsmStatement(node->right);
	}
	else if (node->op == CompiledTree::NodeType::Call) {
		toAsmPushVars(node->left, 0);
		outfile << "call :" << node->value << " \n";
		toAsmStatement(node->right);
	}
	else if (node->op == CompiledTree::NodeType::Function) {
		outfile << ":" << node->value << " \n";
		scopes.push_back({});
		toAsmPopVars(node->left, 0);
		toAsmStatement(node->right->left);
		outfile << "ret \n";
		freeScope(scopes[scopes.size() - 1]);
		toAsmStatement(node->right->right);
	}
	else if (node->op == CompiledTree::NodeType::Return) {
		outfile << "ret \n";
		toAsmStatement(node->right);
	}
	else if (node->op == CompiledTree::NodeType::Halt) {
		outfile << "jmp :_THE_END_ \n";
		toAsmStatement(node->right);
	}
}

void TwitchLangCompiler::toAsmDeclareLocal(const std::string& name) {
	int reg = getMinFreeReg();
	scopes[scopes.size() - 1].insert({ name, reg });
}

int TwitchLangCompiler::getMinFreeReg() {
	for (int i = 1; i < 128; ++i) {
		if (!is_reg_reserved[i]) {
			is_reg_reserved[i] = true;
			return i;
		}
	}
}

void TwitchLangCompiler::freeScope(const std::map<std::string, int>& scope) {
	for (auto& var : scope) {
		is_reg_reserved[var.second] = false;
	}
}

int TwitchLangCompiler::findVarReg(const std::string& name) {
	for (int i = scopes.size() - 1; i >= 0; --i) {
		if (scopes[i].find(name) != scopes[i].end()) {
			return scopes[i][name];
		}
	}
}

void TwitchLangCompiler::toAsmPushVars(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node, int depth) {
	if (!node) {
		return;
	}
	outfile << "pushr r" << 196 + depth << " \n";
	toAsmPushVars(node->right, depth + 1);
}


void TwitchLangCompiler::toAsmPopVars(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node, int depth) {
	if (!node) {
		return;
	}
	toAsmPopVars(node->right, depth+1);
	outfile << "popr r" << 196 + depth << " \n";
}

void TwitchLangCompiler::calcExpression(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node, int reg) {
	if (node->op == CompiledTree::NodeType::Variable) {
		outfile << "movl r" << reg << " r" << findVarReg(node->value) << "\n";
		return;
	}
	if (node->op == CompiledTree::NodeType::Number) {
		outfile << "movl r" << reg << " " << node->value << "\n";
		return;
	}
	calcExpression(node->left, reg + 1);
	outfile << "movl r" << reg << " r" << reg + 1 << "\n";
	calcExpression(node->right, reg + 1);
	switch (node->op)
	{
	case CompiledTree::NodeType::Plus:
		outfile << "addl r" << reg << " r" << reg + 1 << "\n";
		break;
	case CompiledTree::NodeType::Minus:
		outfile << "subl r" << reg << " r" << reg + 1 << "\n";
		break;
	case CompiledTree::NodeType::Div:
		outfile << "divl r" << reg << " r" << reg + 1 << "\n";
		break;
	case CompiledTree::NodeType::Mul:
		outfile << "mull r" << reg << " r" << reg + 1 << "\n";
		break;
	default:
		break;
	}
}


std::string TwitchLangCompiler::peek_word() {
	if (codeFlow.eof()) {
		return "END";
	}
  std::string result;
  std::streampos p_orig = codeFlow.tellg();
  codeFlow >> result;
  codeFlow.seekg(p_orig);
  return result;
}

int TwitchLangCompiler::get_line_number(){
	code = codeFlow.str();
	return std::count(code.begin(), code.begin() + codeFlow.tellg(), '\n');
}


/*

	PARSING SECTION

*/
void TwitchLangCompiler::parseProgramm() {
	int n = 0;
	auto cur = active_node;
	parseStatement();
	n = get_line_number();
	while (!codeFlow.eof() && (peek_word() != "CarlSmile") && (peek_word() != "")) {
		if (*active_node) {
			active_node = &active_node->get()->right;
		}
		parseStatement();
		n = get_line_number();
	}
	//assert(codeFlow.eof());
}


// TODO :: add swap_child function
void TwitchLangCompiler::parseStatement() {
	//auto token = peak_word();
	std::string token;
	codeFlow >> token;
	if ( token == "Kappa" ){
		do {
			codeFlow >> token;
		} while (token != "Kappa");
		return;
	}
	*active_node = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Halt));

	if ( token == "KonCha" ) {
		codeFlow >> token;

		if( keyWords.find(token) != keyWords.end() ){
			throw std::runtime_error(std::string("Error on line ") + 
				std::to_string(get_line_number()) + 
				" : " +
				"variable name can't be a keyword");
		} 

		active_node->get()->op = CompiledTree::NodeType::Declaration;
		active_node->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
		active_node->get()->left.get()->value = token;
		
		/*token = peek_word();
		while ( token == "," ){
			codeFlow >> token;
			auto node = comptree.insert(CompiledTree::NodeType::Declaration);
			node->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
			node->get()->left.get()->value = token;		
		}*/

		parseSeparator();

	} else if ( token == "MrDestructoid" ) {
		
		codeFlow >> token;
		auto node = active_node;
		node->get()->op = CompiledTree::NodeType::Input;
		node->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
		node->get()->left.get()->value = token;
		parseSeparator();

	} else if ( token == "SwiftRage" ) {
		
		codeFlow >> token;
		auto node = active_node;
		node->get()->op = CompiledTree::NodeType::Output;
		node->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
		node->get()->left.get()->value = token;
		
		parseSeparator();

	} else if ( token == "WutFace" ) {
		// WutFace PowerUpL Expression PowerUpR FeelsGoodMan CoolStoryBob Programm CarlSmile FeelsBadMan CoolStoryBob Programm CarlSmile 
		//codeFlow >> token;

		auto ifnode = active_node;
		ifnode->get()->op = CompiledTree::NodeType::If;

		assertEquality("PowerUpL");
		//active_node = &ifnode->get()->left;
		//parseExpression();
		std::string var1, var2, compar;
		codeFlow >> var1 >> compar >> var2;
		if (compar == "<") {
			ifnode->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Less));
		}
		else if (compar == "SeemsGood") {
			ifnode->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Equal));
		}
		else {
			ifnode->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::More));
		}	
		ifnode->get()->left->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
		ifnode->get()->left->left->value = var1;
		ifnode->get()->left->right = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
		ifnode->get()->left->right->value = var2;
		assertEquality("PowerUpR");
		
		
		ifnode->get()->right = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Then));
		auto thennode = &ifnode->get()->right;

		assertEquality("FeelsGoodMan");
		assertEquality("CoolStoryBob");
		active_node = &thennode->get()->left;
		parseProgramm();
		assertEquality("CarlSmile");
		
		thennode->get()->right = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Else));
		auto elsenode = &thennode->get()->right;

		assertEquality("FeelsBadMan");
		assertEquality("CoolStoryBob");
		active_node = &elsenode->get()->left;
		parseProgramm();
		assertEquality("CarlSmile");

		active_node = elsenode;

	} else if ( token == "Pog" ) {
		
		auto whilenode = active_node;
		whilenode->get()->op = CompiledTree::NodeType::While;

		assertEquality("PowerUpL");
		std::string var1, var2, compar;
		codeFlow >> var1 >> compar >> var2;
		if (compar == "<") {
			whilenode->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Less));
		}
		else if (compar == "SeemsGood") {
			whilenode->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Equal));
		}
		else {
			whilenode->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::More));
		}
		whilenode->get()->left->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
		whilenode->get()->left->left->value = var1;
		whilenode->get()->left->right = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
		whilenode->get()->left->right->value = var2;
		assertEquality("PowerUpR");

		whilenode->get()->right = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Then));
		auto donode = &whilenode->get()->right;
		
		assertEquality("CoolStoryBob");
		active_node = &donode->get()->left;
		parseProgramm();
		assertEquality("CarlSmile");
		active_node = &donode->get()->right;
	}
	else if (token == "gachiBASS") {
		codeFlow >> token;
		auto funname = token;
		assertEquality("PowerUpL");
		active_node->get()->op = CompiledTree::NodeType::Function;
		active_node->get()->value = funname;

		codeFlow >> token;
		active_node->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Semicolom));
		active_node->get()->left->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
		active_node->get()->left->left->value = token;
		auto cur = &active_node->get()->left;
		while (peek_word() == ",")
		{
			codeFlow >> token;
			codeFlow >> token;
			cur->get()->right = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Semicolom));
			cur->get()->right->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
			cur->get()->right->left->value = token;
			cur = &cur->get()->right;
		}
		//
		active_node->get()->right = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Then));
		auto funnode = active_node;
		assertEquality("PowerUpR");
		assertEquality("CoolStoryBob");
		active_node = &funnode->get()->right->left;
		parseProgramm();
		assertEquality("CarlSmile");
		active_node = &funnode->get()->right->right;
	} 
	else if (token == "FeelsAmazingMan") {
		active_node->get()->op = CompiledTree::NodeType::Return;
	}
	else if (token == "FuckYea") {
		return;
	}
	else {
		auto word = token;
		codeFlow >> token;
		if (token == "D:") {
			auto cur = active_node;
			active_node->get()->op = CompiledTree::NodeType::Assign;
			active_node->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::To));
			active_node->get()->left->right = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
			active_node->get()->left->right->value = word;
			active_node = &active_node->get()->left->left;
			parseExpression();
	
			parseSeparator();
			active_node = cur;
		}
		else if (token == "PowerUpL") {
			codeFlow >> token;
			active_node->get()->value = word;
			active_node->get()->op = CompiledTree::NodeType::Call;
			active_node->get()->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Semicolom));
			active_node->get()->left->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
			active_node->get()->left->left->value = token;
			auto cur = &active_node->get()->left;
			while (peek_word() == ",")
			{
				codeFlow >> token;
				codeFlow >> token;
				cur->get()->right = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Semicolom));
				cur->get()->right->left = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
				cur->get()->right->left->value = token;
				cur = &cur->get()->right;
			}
			assertEquality("PowerUpR");

			parseSeparator();
			active_node = &active_node->get()->right;
		}
		else {
			throw std::runtime_error("Error while parsing line " + get_line_number());
		}
	}
}

/*
	Expression -> Expression1 { Exp1op Expression1 } *
	Exp1op -> [+-]
	Expression1 -> Expression2 { Exp2op Expression2 } *
	Exp2op -> [*\/]
	Expression2 -> ( Expression ) | Word | Number
*/
void TwitchLangCompiler::parseExpression() {
	auto cur = active_node;
	*cur = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Halt));
	parseExpression1();
	auto nxt = peek_word();	
	while (isExp1op(nxt)) {
		auto temp = std::move(*cur);
		*cur = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Halt));
		cur->get()->left = std::move(temp);
		active_node = cur;
		parseExpression1operator();
		active_node = &cur->get()->right;
		parseExpression1();
		nxt = peek_word();
	}
}

void TwitchLangCompiler::parseExpression1() {
	auto cur = active_node;
	*cur = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Halt));
	parseExpression2();
	auto nxt = peek_word();
	while (isExp2op(nxt)) {
		auto temp = std::move(*cur);
		*cur = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Halt));
		cur->get()->left = std::move(temp);
		active_node = cur;
		parseExpression2operator();
		active_node = &cur->get()->right;
		parseExpression2();
		nxt = peek_word();
	}
}

void TwitchLangCompiler::parseExpression1operator() {
	std::string token;
	codeFlow >> token;
	if (!(isExp1op(token))) {
		throw std::runtime_error("Expected riPepperonis or FBChargers on line " + std::to_string(get_line_number()) + " but got " + token);
	}
	if (token == "riPepperonis") {
		active_node->get()->op = CompiledTree::NodeType::Plus;
	}
	if (token == "FBChargers") {
		active_node->get()->op = CompiledTree::NodeType::Minus;
	}
}

void TwitchLangCompiler::parseExpression2() {
	std::string token;
	codeFlow >> token;
	if (token == "PowerUpL") {
		parseExpression();
		assertEquality("PowerUpR");
	}
	else if (is_number(token)) {
		auto numnode = active_node;
		*numnode = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Number));
		numnode->get()->value = token;
	}
	else {
		auto numnode = active_node;
		*numnode = std::make_unique<CompiledTree::Node>(CompiledTree::Node(CompiledTree::NodeType::Variable));
		numnode->get()->value = token;
	}
}

void TwitchLangCompiler::parseExpression2operator() {
	std::string token;
	codeFlow >> token;
	if (!(isExp2op(token))) {
		throw std::runtime_error("Expected CurseLit or Squid4 on line " + std::to_string(get_line_number()) + " but got " + token);
	}
	if (token == "CurseLit") {
		active_node->get()->op = CompiledTree::NodeType::Mul;
	}
	if (token == "Squid4") {
		active_node->get()->op = CompiledTree::NodeType::Div;
	}
}


void TwitchLangCompiler::parseSeparator() {
	assertEquality("SMOrc");
}

void TwitchLangCompiler::assertEquality(const std::string& s) {
	std::string value;
	codeFlow >> value;
	if (value != s) {
		throw std::runtime_error(s + " expected on line " + std::to_string(get_line_number()) + " but got " + value);
	}
}

// riPepperonis CurseLit FBChargers Squid4 
bool TwitchLangCompiler::isExp2op(const std::string& s) {
	return (s == "CurseLit") || (s == "Squid4");
}

bool TwitchLangCompiler::isExp1op(const std::string& s) {
	return (s == "riPepperonis") || (s == "FBChargers");
}


bool TwitchLangCompiler::isString(const std::string& s) {
	return (s[0] == '\"') || (s[s.size()-1] == '\"');
}

bool TwitchLangCompiler::is_number(const std::string& s) {
	bool is_correct = true;
	for (char el : s) {
		if (!isdigit(el)) {
			is_correct = false;
			break;
		}
	}
	return is_correct;
}

TwitchLangCompiler::CompiledTree::Node::Node(TwitchLangCompiler::CompiledTree::NodeType t) {
	op = t;
}

void TwitchLangCompiler::swap_children(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>* n) {
	n->get()->left = std::move(n->get()->right);
}

std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>* TwitchLangCompiler::CompiledTree::insert(TwitchLangCompiler::CompiledTree::NodeType t) {
	if (rightmost == nullptr) {
		root = std::make_unique<Node>(Node(t));
		rightmost = &root;
		return rightmost;
	}
	rightmost->get()->right = std::make_unique<Node>(Node(t));
	rightmost = &rightmost->get()->right;
	return rightmost;
}

std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>* TwitchLangCompiler::CompiledTree::insert_by_ptr(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>* opnode, TwitchLangCompiler::CompiledTree::NodeType t) {
	auto temp = std::move(*opnode);
	*opnode = std::make_unique<CompiledTree::Node>(CompiledTree::Node(t));
	opnode->get()->left = std::move(temp);
	rightmost = &opnode->get()->right;
	return rightmost;
}