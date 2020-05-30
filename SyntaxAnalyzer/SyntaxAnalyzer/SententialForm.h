#pragma once
#include <vector>
#include <iostream>

using namespace std;

enum class SYMBOL {
	// terminals
	VTYPE = 0, NUM, FLOAT, LITERAL, ID,
	IF, ELSE, WHILE, FOR, RETURN,
	ADDSUB, MULTDIV, ASSIGN, COMP, SEMI,
	COMMA, LPAREN, RPAREN, LBRACE, RBRACE,
	EPSILON, ENDMARKER,

	// non-termianls
	CODE, VDECL, FDECL, ARG, MOREARGS,
	BLOCK, STMT, ASSIGN_NON, RHS, EXPR,
	TERM, FACTOR, COND, RETURN_NON, ELSE_NON,
	START, E, T//temp
};

class SententialForm
{
private:
	vector<SYMBOL> sentential;

public:
	SententialForm();
	~SententialForm();
	void codeToSentence(vector<string> );
	void pushBack(SYMBOL);
	vector<SYMBOL> getSentence();
};