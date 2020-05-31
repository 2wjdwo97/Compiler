#pragma once
#include "Exception.h"
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

class Exception;

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

typedef struct {
	string name;
	SYMBOL value;
} SymbolInfo;

const vector<vector<SymbolInfo>> table = {
	{{"VTYPE", SYMBOL::VTYPE}},
	{{"NUM", SYMBOL::NUM}},
	{{"FLOAT", SYMBOL::FLOAT}},
	{{"LITERAL", SYMBOL::LITERAL}},
	{{"ID", SYMBOL::ID}},
	{{"KEYWORD", SYMBOL::EPSILON}, {"if", SYMBOL::IF}, {"else", SYMBOL::ELSE}, {"whlie", SYMBOL::WHILE}, {"for", SYMBOL::FOR}, {"return", SYMBOL::RETURN}},
	{{"ARITHM", SYMBOL::EPSILON}, {"+", SYMBOL::ADDSUB}, {"-", SYMBOL::ADDSUB}, {"*", SYMBOL::MULTDIV}, {"/", SYMBOL::MULTDIV}},
	{{"ASSIGN", SYMBOL::ASSIGN}},
	{{"COMP", SYMBOL::COMP}},
	{{"SEMI", SYMBOL::SEMI}},
	{{"COMMA", SYMBOL::COMMA}},
	{{"PAREN", SYMBOL::EPSILON}, {"(", SYMBOL::LPAREN}, {")", SYMBOL::RPAREN}},
	{{"BRACE", SYMBOL::EPSILON}, {"{", SYMBOL::LBRACE}, {"}", SYMBOL::RBRACE}}
};