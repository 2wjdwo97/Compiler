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
	ENDMARKER,

	// non-termianls
	CODE, VDECL, FDECL, ARG, MOREARGS,
	BLOCK, STMT, ASSIGN_NON, RHS, EXPR,
	TERM, FACTOR, COND, RETURN_NON, ELSE_NON,
	START,
	
	// Wrong input
	ERROR
};

typedef struct {
	string tokenValue;
	int lineNumber;
} Symbol_Info;

class Exception;

class SententialForm
{
private:
	vector<SYMBOL> sentential;	// sentential form used in the SLR parsing
	vector<Symbol_Info> errorData;

	bool isInteger(const string& s);

public:
	SententialForm();
	~SententialForm();

	void tokensToSentence(const vector<string> );

	void pushBack(const SYMBOL);
	void pushBackErrorData(const Symbol_Info );

	void erase(int, int);
	void eraseErrorData();
	void insert(int, SYMBOL);
	
	vector<Symbol_Info> getErrorData();
	vector<SYMBOL>& getSentence();
};

/* Translation Table (symbol_table to sentence) */
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
	{{"KEYWORD", SYMBOL::ERROR}, {"if", SYMBOL::IF}, {"else", SYMBOL::ELSE}, {"while", SYMBOL::WHILE}, {"for", SYMBOL::FOR}, {"return", SYMBOL::RETURN}},
	{{"ARITHM", SYMBOL::ERROR}, {"+", SYMBOL::ADDSUB}, {"-", SYMBOL::ADDSUB}, {"*", SYMBOL::MULTDIV}, {"/", SYMBOL::MULTDIV}},
	{{"ASSIGN", SYMBOL::ASSIGN}},
	{{"COMP", SYMBOL::COMP}},
	{{"SEMI", SYMBOL::SEMI}},
	{{"COMMA", SYMBOL::COMMA}},
	{{"PAREN", SYMBOL::ERROR}, {"(", SYMBOL::LPAREN}, {")", SYMBOL::RPAREN}},
	{{"BRACE", SYMBOL::ERROR}, {"{", SYMBOL::LBRACE}, {"}", SYMBOL::RBRACE}}
};