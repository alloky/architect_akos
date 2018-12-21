#include "expression_parser.h"

#include <sstream>
#include <stack>


// ( ( x ^ 3 ) - ( 2 * ( x ^ 2 ) ) ) - ( x + 1 )
// x  + ( cos( x ) / sin( x ) )

Tree ExpressionParser::parse_expression(const std::string& s) {
	std::stringstream flow(s);
	std::string token;
	Tree expression;
	expression.root = std::make_unique<Tree::Node>(Tree::Node());
	auto cur = &(expression.root);

	std::stack<std::unique_ptr<Tree::Node>*> st;
	st.push(&(expression.root));

	while (flow >> token) {
		if (delimiters.find(token) != delimiters.end()) {
			if (!delimiters[token].second) {
				cur = st.top();
				st.pop();
				
				auto left_child = std::make_unique<Tree::Node>(Tree::Node());
				left_child->left = std::move(cur->get()->left);
				left_child->right = std::move(cur->get()->right);
				left_child->op = cur->get()->op;
				left_child->value = cur->get()->value;

				cur->get()->op = delimiters[token].first;
				cur->get()->right = std::make_unique<Tree::Node>(Tree::Node());
				cur->get()->left = std::move(left_child);

				st.push(cur);
				cur = &cur->get()->right;
			}
			else {
				cur->get()->op = delimiters[token].first;
				cur->get()->left = std::make_unique<Tree::Node>(Tree::Node());
				st.push(cur);
				cur = &cur->get()->left;
			}
		} else if (token == "(") {
			st.push(cur);
		} else if (token == ")") {
			auto top = st.top();
			st.pop();
			cur = top;
		}
		else if (token == "x") {
			cur->get()->op = Tree::NodeType::X;
		}
		else {
			double val = std::stod(token);
			cur->get()->op = Tree::NodeType::CONST;
			cur->get()->value = val;
		}
	}

	expression.root = std::move(*st.top());
	return expression;
}