#pragma once
#include <memory>
#include <map>
#include <functional>
#include <string>

class Tree {
public:
	enum NodeType {
		CONST,
		X,
#define DEF_NODE_TYPE(mono, delim, Name, Tex) Name ## _OP,
#include "node-types-defs.h"
#undef DEF_NODE_TYPE
	};



	class Node {
	public:
		explicit Node(Tree::NodeType _t, std::unique_ptr<Tree::Node>&& _l, std::unique_ptr<Tree::Node>&& _r);
		explicit Node(Node&& _x, Node* _p);
		explicit Node(double x);
		Node();

		std::string toGraph();
		std::string toTex();
		//Node(Node&&) = delete;
		//Node(Node&) = delete;

		std::unique_ptr<Node> left, right;
		Node* parent;
		NodeType op;
		double value;
	};

	Tree optimize();
	Tree differetiate();

	std::string toGraph();
	std::string toTex();

	std::unique_ptr<Node> root{ nullptr };
};

std::unique_ptr<Tree::Node> operator +(std::unique_ptr<Tree::Node>&& a, std::unique_ptr<Tree::Node>&& b);
std::unique_ptr<Tree::Node> operator -(std::unique_ptr<Tree::Node>&& a, std::unique_ptr<Tree::Node>&& b);
std::unique_ptr<Tree::Node> operator *(std::unique_ptr<Tree::Node>&& a, std::unique_ptr<Tree::Node>&& b);
std::unique_ptr<Tree::Node> operator /(std::unique_ptr<Tree::Node>&& a, std::unique_ptr<Tree::Node>&& b);


std::unique_ptr<Tree::Node> diff(std::unique_ptr<Tree::Node>& n);
std::unique_ptr<Tree::Node> copy(std::unique_ptr<Tree::Node>& n, Tree::Node* _p);

std::unique_ptr<Tree::Node> optimize_tree(std::unique_ptr<Tree::Node>& n);

bool optimize_mull(std::unique_ptr<Tree::Node>& n);
bool optimize_add(std::unique_ptr<Tree::Node>& n);

#define DEF_NODE_TYPE(mono, delim, Name, Tex) \
std::unique_ptr<Tree::Node> Name (std::unique_ptr<Tree::Node>& n);	
#include "node-types-defs.h"
#undef DEF_NODE_TYPE
