#pragma once

#include "Tree.h"

	
std::unique_ptr<Tree::Node> operator +(std::unique_ptr<Tree::Node>&& a, std::unique_ptr<Tree::Node>&& b) {
	return std::unique_ptr<Tree::Node>(new Tree::Node(Tree::NodeType::PLUS_OP, std::move(a), std::move(b)));
}

std::unique_ptr<Tree::Node> operator -(std::unique_ptr<Tree::Node> &&a, std::unique_ptr<Tree::Node>&& b) {
	return std::unique_ptr<Tree::Node>(new Tree::Node(Tree::NodeType::MINUS_OP, std::move(a), std::move(b)));
}
std::unique_ptr<Tree::Node> operator *(std::unique_ptr<Tree::Node> &&a, std::unique_ptr<Tree::Node> &&b) {
	return std::unique_ptr<Tree::Node>(new Tree::Node(Tree::NodeType::MUL_OP, std::move(a), std::move(b)));
}
std::unique_ptr<Tree::Node> operator /(std::unique_ptr<Tree::Node> &&a, std::unique_ptr<Tree::Node> &&b) {
	return std::unique_ptr<Tree::Node>(new Tree::Node(Tree::NodeType::DIV_OP, std::move(a), std::move(b)));
}

Tree::Node::Node(Tree::NodeType _t, std::unique_ptr<Tree::Node>&& _l, std::unique_ptr<Tree::Node>&& _r) {
	left = std::move(_l);
	right = std::move(_r);
	op = _t;
}

Tree::Node::Node() {
}

Tree::Node::Node(Tree::Node&& _x, Tree::Node* _p) {
	op = _x.op;
	value = _x.value;
	parent = _p;
}
Tree::Node::Node(double x) {
	//left = std::move(std::make_unique<Tree::Node>(Tree::Node()));
	//right = std::move(std::make_unique<Tree::Node>(Tree::Node()));
	op = Tree::NodeType::CONST;
	value = x;
}

std::string Tree::Node::toGraph() {
	std::string label;
	std::string tmp;
	switch (op) {
	case Tree::NodeType::CONST:
		tmp = std::to_string(value);
		label = tmp.erase(tmp.find_last_not_of('0') + 1, std::string::npos);
		break;
	case Tree::NodeType::X:
		label = "x";
		break;
#define DEF_NODE_TYPE(mono, delim, Name, Tex) \
	case Tree::NodeType:: ## Name ##_OP :\
		label = #Name; \
		break;
#include "node-types-defs.h"
#undef DEF_NODE_TYPE	
	default:
		label = "broken label";
		break;
	}
	std::string result = "";
	result += std::to_string(reinterpret_cast<int>(this)) + " [label = \"" + label + "\"] \n";
	if (left.get() != nullptr) {
		result += std::to_string(reinterpret_cast<int>(this)) + " -> " + std::to_string(reinterpret_cast<int>(left.get())) + "\n";
		result += left.get()->toGraph();
	}
	if (right.get() != nullptr) {
		result += std::to_string(reinterpret_cast<int>(this)) + " -> " + std::to_string(reinterpret_cast<int>(right.get())) + "\n";
		result += right.get()->toGraph();
	}
	return result;
}

std::string Tree::Node::toTex() {
	//return "( " + std::string("\\frac{ ") + left->toTex() + " }{ " + right->toTex() + " }" + " )";
	std::string tmp;
	switch (op)
	{
	case Tree::NodeType::CONST:
		tmp = std::to_string(value);
		return " " + tmp.erase(tmp.find_last_not_of('0') + 1, std::string::npos) + " ";
	case Tree::NodeType::X:
		return " x ";
#define DEF_NODE_TYPE(mono, delim, Name, Tex) \
	case Tree::NodeType:: ## Name ##_OP :\
		return "( " + Tex + " )";
#include "node-types-defs.h"
#undef DEF_NODE_TYPE	
	default:
		return " broken ";
	}
}

std::unique_ptr<Tree::Node> copy(std::unique_ptr<Tree::Node>& n, Tree::Node* _p=nullptr) {
	std::unique_ptr<Tree::Node> root(new Tree::Node(std::move(*(n.get())), _p));
	root.get()->parent = _p;
	if (n.get()->left != nullptr) {
		root.get()->left = std::move(copy(n.get()->left, root.get()));
	}
	if (n.get()->right != nullptr) {
		root.get()->right = std::move((copy(n.get()->right, root.get())));
	}

	return root;
}

std::unique_ptr<Tree::Node> diff(std::unique_ptr<Tree::Node>& n) {
	switch (n->op)
	{
	case Tree::NodeType::X:
		return std::make_unique<Tree::Node>(Tree::Node(1.));
		break;
	case Tree::NodeType::CONST:
		return std::make_unique<Tree::Node>(Tree::Node(0.));
		break;
#define DEF_NODE_TYPE(mono, delim, Name, Tex) \
	case Tree::NodeType:: ## Name ##_OP :\
		return Name(n);
#include "node-types-defs.h"
#undef DEF_NODE_TYPE	
	default:
		throw std::exception("Not implemented");
	}
}

Tree Tree::differetiate() {
	Tree ret;
	ret.root = std::move(diff(root));
	return ret;
}

Tree Tree::optimize() {
	Tree ret;
	ret.root = std::move(optimize_tree(root));
	return ret;
}

std::string Tree::toGraph() {
	return root->toGraph();
}

std::string Tree::toTex() {
	std::string result;
	result =
		"\\documentclass[12pt]{proc}        \n"
		"\\title{Automatic differentiator.} \n"
		"\\usepackage{graphicx} \n"
		"\\begin{document}                  \n"
		"\\author{Ivanin.V.A. 697}          \n"
		"\\maketitle                          \n";
		
	result += "Input function : \n\n";

	result += "\\begin{equation} \n"
		"\\resizebox{ .9 \\textwidth }{!} \n"
		"{ \n";
	result += "$ f(x) = " + root->toTex() + " $\n\n";
	result += "} \n"
		"\\end{equation} \n";

	Tree optimized_diff = differetiate();
	optimized_diff = optimized_diff.optimize();
	result += "Output function : \n\n";
	
	result += "\\begin{equation} \n"
		      "\\resizebox{ .9 \\textwidth }{!} \n"
		      "{ \n";
	result += " $ f'(x) = " + optimized_diff.root->toTex() + " $\n\n";
	result += "} \n"
		      "\\end{equation} \n";

	result += "\\end{document}";
	return result;
}

std::unique_ptr<Tree::Node> optimize_tree(std::unique_ptr<Tree::Node>& n) {
	bool is_changed = true;
	while (is_changed == true) {
		is_changed = optimize_add(n);
		is_changed |= optimize_mull(n);
	}
	return std::move(n);
}

bool optimize_add(std::unique_ptr<Tree::Node>& n) {
	bool is_changed = false;
	if (n->left.get() != nullptr) {
		is_changed |= optimize_add(n->left);
	}
	if (n->right.get() != nullptr) {
		is_changed |= optimize_add(n->right);
	}
	if (n->op == Tree::NodeType::PLUS_OP) {
		if (n->left.get()->op == Tree::NodeType::CONST && n->left.get()->value == 0.) {
			n = std::move(n->right);
			return true;
		}
		if (n->right.get()->op == Tree::NodeType::CONST && n->right.get()->value == 0.) {
			n = std::move(n->left);
			return true;
		}
	}
	if (n->op == Tree::NodeType::MINUS_OP) {
		if (n->right.get()->op == Tree::NodeType::CONST && n->right.get()->value == 0.) {
			n = std::move(n->left);
			return true;
		}
	}
	return is_changed;
}

bool optimize_mull(std::unique_ptr<Tree::Node>& n) {
	bool is_changed = false;
	if (n->left.get() != nullptr) {
		is_changed |= optimize_mull(n->left);
	}
	if (n->right.get() != nullptr) {
		is_changed |= optimize_mull(n->right);
	}
	if (n->op == Tree::NodeType::MUL_OP) {
		if (n->left.get()->op == Tree::NodeType::CONST && n->left.get()->value == 0.) {
			n = std::move(n->left);
			return true;
		}
		if (n->right.get()->op == Tree::NodeType::CONST && n->right.get()->value == 0.) {
			n = std::move(n->right);
			return true;
		}
	}
	if ((n->left.get() != nullptr) && (n->right.get() != nullptr)) {
		if (n->left.get()->op == Tree::NodeType::CONST
			&& n->right.get()->op == Tree::NodeType::CONST) {
			switch (n->op)
			{
			case Tree::PLUS_OP:
				n->left.get()->value = n->left.get()->value + n->right.get()->value;
				break;
			case Tree::MINUS_OP:
				n->left.get()->value = n->left.get()->value - n->right.get()->value;
				break;
			case Tree::MUL_OP:
				n->left.get()->value = n->left.get()->value * n->right.get()->value;
				break;
			case Tree::DIV_OP:
				n->left.get()->value = n->left.get()->value / n->right.get()->value;
				break;
			default:
				return is_changed;
			}
			n = std::move(n->left);
			return true;
		}
	}
	return is_changed;
}

std::unique_ptr<Tree::Node> PLUS(std::unique_ptr<Tree::Node>& n) {
	return diff(n->left) + diff(n->right);
}

std::unique_ptr<Tree::Node> MINUS(std::unique_ptr<Tree::Node>& n) {
	return diff(n->left) - diff(n->right);
}

std::unique_ptr<Tree::Node> MUL(std::unique_ptr<Tree::Node>& n) {
	return  diff(n->left) * (copy(n->right)) + diff(n->right)*(copy(n->left));
}
std::unique_ptr<Tree::Node> DIV(std::unique_ptr<Tree::Node>& n) {
	return (diff(n->left) * (copy(n->right)) - diff(n->right)*(copy(n->left))) / (copy(n->right) * copy(n->right));
}
std::unique_ptr<Tree::Node> SIN(std::unique_ptr<Tree::Node>& n) {
	return std::move(diff(n->left)) * std::move(std::make_unique<Tree::Node>(Tree::Node(Tree::NodeType::COS_OP, copy(n), std::unique_ptr<Tree::Node>(nullptr))));
}
std::unique_ptr<Tree::Node> COS(std::unique_ptr<Tree::Node>& n) {
	return std::move(std::make_unique<Tree::Node>(Tree::Node(-1.))) * diff(n->left) * std::move(std::make_unique<Tree::Node>(Tree::Node(Tree::NodeType::SIN_OP, copy(n), std::unique_ptr<Tree::Node>(nullptr))));
}
std::unique_ptr<Tree::Node> POW(std::unique_ptr<Tree::Node>& n) {
	return std::move(diff(n->left)) * std::move(std::make_unique<Tree::Node>(Tree::Node(n->right.get()->value))) * std::move(std::make_unique<Tree::Node>(Tree::Node(Tree::NodeType::POW_OP,
																																 copy(n->left), 
																																 std::make_unique<Tree::Node>(Tree::Node(n->right.get()->value - 1.)))));
}

