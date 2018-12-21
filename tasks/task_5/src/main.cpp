#include <string>
#include <fstream>

#include "logging.h"
#include "expression_parser.h"

// ( 1 + ( ( 2 * x ) / x ) ) - 3
// ( cos( sin( x ^ 3 ) ) / sin( ( 3 * x ) - 5 ) ) * x 
int main() {
	std::string inp;
	std::getline(std::cin, inp);
	ExpressionParser p;
	auto tree = p.parse_expression(inp);
	
	std::ofstream file;

	file.open("some1.gv");
	file << "digraph G {\n";
	file << tree.root.get()->toGraph();
	file << "}\n";
	file.close();

	auto difftree = tree.differetiate();
	
	file.open("some2.gv");
	file << "digraph G {\n";
	file << difftree.toGraph();
	file << "}\n";
	file.close();

	difftree = difftree.optimize();

	file.open("some3.gv");
	file << "digraph G {\n";
	file << difftree.toGraph();
	file << "}\n";
	file.close();

	file.open("report.tex");
	file << tree.toTex();

	return 0;
}