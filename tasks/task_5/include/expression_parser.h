#pragma once

#include <string>
#include <Tree.h>
#include <map>
#include <utility>

class ExpressionParser {
	std::map<const std::string, std::pair<Tree::NodeType, bool>> delimiters {
#define DEF_NODE_TYPE(mono, delim, Name, Tex) { delim ,  { Tree::NodeType:: ## Name ## _OP , mono } },
#include "node-types-defs.h"
#undef DEF_NODE_TYPE
	};	
public:
	Tree parse_expression(const std::string& s);
};