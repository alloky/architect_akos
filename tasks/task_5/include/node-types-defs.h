
DEF_NODE_TYPE(false, "+" ,   PLUS,  left->toTex() + " + " + right->toTex() )
DEF_NODE_TYPE(false, "-" ,   MINUS, left->toTex() + " - " + right->toTex())
DEF_NODE_TYPE(false, "*" ,   MUL,   left->toTex() + " " + right->toTex())
DEF_NODE_TYPE(false, "/" ,   DIV,   std::string("\\frac{ ") + left->toTex() + " }{ " + right->toTex() + " }")
DEF_NODE_TYPE(false, "^" ,   POW, std::string("( ") + left->toTex() + " )^{ " + right->toTex() + " }")

DEF_NODE_TYPE(true, "sin(", SIN, std::string("\\sin( ") + left->toTex() + " )")
DEF_NODE_TYPE(true, "cos(", COS, std::string("\\cos( ") + left->toTex() + " )")

