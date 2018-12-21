#pragma once

#include <string>
#include <set>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <ctype.h>
#include <map>
#include <vector>

// TwitchLang grammar
/*

	Programm -> Statement { SMOrc Statement }
	Statement -> Kappa Text | 
				 KonCha Word { , Word } | 
				 MrDestructoid Word |
				 Word D: Experssion |
				 WutFace PowerUpL Expression PowerUpR FeelsGoodMan CoolStoryBob Programm CarlSmile FeelsBadMan CoolStoryBob Programm CarlSmile |
				 4Head PowerUpL Statement SMOrc Expression SMOrc Statement PowerUpR CoolStoryBob Programm CarlSmile |
				 SwiftRage " Text " |
				 Pog PowerUpL Expression  PowerUpR CoolStoryBob Programm CarlSmile 
	Expression -> Expression1 { Exp1op Expression1 } *
	Exp1op -> [+-]
	Expression1 -> Expression2 { Exp2op Expression2 } *
	Exp2op -> [*\/]
	Expression2 -> ( Expression ) | Word | Number
	Word -> [A-Za-z][a-z]* ( != ключевому )
	Number -> [1-9][0-9]*

*/


class TwitchLangCompiler {
public:
	std::string compile(const std::string& _code, const std::string& dest);
	void compileToAsm(const std::string& dest);
	int get_line_number();
	
	void parseProgramm();
	std::string peek_word();
	void parseStatement();
	void parseExpression();
	void parseExpression1();
	void parseExpression2operator();
	void parseExpression1operator();
	void parseExpression2();
	void parseSeparator();
	void assertEquality(const std::string & s);
	bool isExp2op(const std::string & s);
	bool isExp1op(const std::string & s);
	bool isString(const std::string & s);
	bool is_number(const std::string & s);
private:

	class CompiledTree {
	public:
		enum NodeType {
			Programm,
			Statement,

			Separator,
			Variable,
			String,

			Call,
			Declaration,
			Input,
			Output,
			If,
			Then,
			Else,
			While,
			Function,

			Assign,
			Plus,
			Minus,
			Mul,
			Div,

			More,
			Equal,
			Less,

			Halt,
			Return,
			To,
			Semicolom,

			Expression,
			Expression1,
			Exp1op,
			Expression2,
			Exp2op,
			Word,
			Number
		};

		class Node {
		public:
			Node(CompiledTree::NodeType t);

			std::unique_ptr<Node> left, right;
			Node* parent;
			NodeType op;
			std::string value;
		};

		std::unique_ptr<Node>* insert(NodeType t);
		std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>* insert_by_ptr(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>* opnode, TwitchLangCompiler::CompiledTree::NodeType t);
		std::unique_ptr<Node> root{ nullptr };
		std::unique_ptr<Node>* rightmost;
	};

	std::stringstream codeFlow;
	std::ofstream outfile;
	std::string code;

	CompiledTree comptree;

	void swap_children(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>* n);
	void toAsm(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node);

	void toAsmStatement(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node);

	void toAsmDeclareLocal(const std::string & name);

	int getMinFreeReg();

	void freeScope(const std::map<std::string, int>& scope);

	int findVarReg(const std::string & name);

	void toAsmPushVars(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node, int depth);

	void toAsmDeclare(const std::string & name);

	void toAsmPopVars(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node, int  depth);

	void calcExpression(std::unique_ptr<TwitchLangCompiler::CompiledTree::Node>& node, int reg);


	std::set<std::string> keyWords = {
		"Kappa",
		"KonCha",
		"WutFace",
		"4Head",
		"SMOrc",
		"Pog",
		"PowerUpL",
		"PowerUpR",
		"SwiftRage",
		"MrDestructoid",
		"CoolStoryBob",
		"CarlSmile"
	};
	//std::map<std::string, int> var_to_reg;
	std::vector<std::map<std::string, int>> scopes;
	bool is_reg_reserved[128];
	//int max_reg{ 1 };
	int label_count{ 0 };
	std::unique_ptr<CompiledTree::Node>* active_node;
	size_t current_pos{ 0 };
};
