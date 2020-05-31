#pragma once
#include "SententialForm.h"
#include "Exception.h"
#include <stack>

class SententialForm;
class Exception;

class SLRparser
{
private:
	stack<int> st;
	SYMBOL nextInputSymbol;
	int splitter;
	vector<SYMBOL> sentential;

	bool isFinish;

public:
	SLRparser();
	~SLRparser();
	void parsing(vector<SYMBOL> );
	template <class T>
	int toIndex(T , vector<T> );
	void changeState(int , int );
	bool getIsFinish();
	bool checkAccept();
};


// shift-reduce 
enum class ACTION {
	SHIFT = 0, REDUCE, EMPTY
};

// a state of SLR parsing table
typedef struct {
	ACTION action;
	int actionNum = -1;
} State;

// SLR parsing table (ACION part)
const vector<SYMBOL> inputSymbolList_Action = { SYMBOL::MULTDIV, SYMBOL::LPAREN, SYMBOL::RPAREN, SYMBOL::ID, SYMBOL::ENDMARKER };
const vector<vector<State>> SLRtable_Action = {
	{{ACTION::EMPTY, 0}, {ACTION::SHIFT, 4}, {ACTION::EMPTY, 0}, {ACTION::SHIFT, 5}, {ACTION::EMPTY, 0}},
	{{ACTION::EMPTY, 0}, {ACTION::EMPTY, 0}, {ACTION::EMPTY, 0}, {ACTION::EMPTY, 0}, {ACTION::REDUCE, 1}},
	{{ACTION::SHIFT, 6}, {ACTION::EMPTY, 0}, {ACTION::REDUCE, 3}, {ACTION::EMPTY, 0}, {ACTION::REDUCE, 3}},
	{{ACTION::EMPTY, 0}, {ACTION::SHIFT, 4}, {ACTION::EMPTY, 0}, {ACTION::SHIFT, 5}, {ACTION::EMPTY, 0}},
	{{ACTION::REDUCE, 5}, {ACTION::EMPTY, 0}, {ACTION::REDUCE, 5}, {ACTION::EMPTY, 0}, {ACTION::REDUCE, 5}},
	{{ACTION::EMPTY, 0}, {ACTION::SHIFT, 4}, {ACTION::EMPTY, 0}, {ACTION::SHIFT, 5}, {ACTION::REDUCE, 1}},
	{{ACTION::EMPTY, 0}, {ACTION::EMPTY, 0}, {ACTION::SHIFT, 9}, {ACTION::EMPTY, 0}, {ACTION::EMPTY, 0}},
	{{ACTION::EMPTY, 0}, {ACTION::EMPTY, 0}, {ACTION::REDUCE, 2}, {ACTION::EMPTY, 0}, {ACTION::REDUCE, 2}},
	{{ACTION::REDUCE, 4}, {ACTION::EMPTY, 0}, {ACTION::REDUCE, 4}, {ACTION::EMPTY, 0}, {ACTION::REDUCE, 4}}
};
// SLR parsing table (GOTO part)
const vector<SYMBOL> inputSymbolList_Goto = { SYMBOL::E, SYMBOL::T, SYMBOL::START };
const vector<vector<int>> SLRtable_Goto = {
	{2, 3, 0},
	{0, 0, 0},
	{0, 0, 0},
	{7, 3, 0},
	{0, 0, 0},
	{8, 3, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
};


typedef struct {
	SYMBOL symbol;
	int length;
} Production;

// context free grammar G
const vector<Production> CFG = {
	{SYMBOL::START, 1},
	{SYMBOL::E, 3},
	{SYMBOL::E, 1},
	{SYMBOL::T, 3},
	{SYMBOL::T, 1}
};

//const vector<Production> CFG = {
//	{NON_TERMINAL::CODE, 2},
//	{NON_TERMINAL::CODE, 2},
//	{NON_TERMINAL::CODE, 0},
//	{NON_TERMINAL::VDECL, 3},
//	{NON_TERMINAL::VDECL, 3},
//	{NON_TERMINAL::ASSIGN, 3},
//	{NON_TERMINAL::FDECL, 9},
//	{NON_TERMINAL::ARG, 3},
//	{NON_TERMINAL::ARG, 0},
//	{NON_TERMINAL::MOREARGS, 4},
//	{NON_TERMINAL::MOREARGS, 0},
//	{NON_TERMINAL::BLOCK, 2},
//	{NON_TERMINAL::BLOCK, 0},
//	{NON_TERMINAL::STMT, 1},
//	{NON_TERMINAL::STMT, 2},
//	{NON_TERMINAL::STMT, 8},
//	{NON_TERMINAL::STMT, 11},
//	{NON_TERMINAL::ELSE, 4},
//	{NON_TERMINAL::ELSE, 0},
//	{NON_TERMINAL::ELSE, 4},
//	{NON_TERMINAL::RHS, 1},
//	{NON_TERMINAL::RHS, 1},
//	{NON_TERMINAL::EXPR, 3},
//	{NON_TERMINAL::EXPR, 1},
//	{NON_TERMINAL::TERM, 3},
//	{NON_TERMINAL::TERM, 1},
//	{NON_TERMINAL::TERM, 3},
//	{NON_TERMINAL::FACTOR, 3},
//	{NON_TERMINAL::FACTOR, 1},
//	{NON_TERMINAL::FACTOR, 1},
//	{NON_TERMINAL::FACTOR, 1},
//	{NON_TERMINAL::COND, 3},
//	{NON_TERMINAL::RETURN, 3},
//};

//const vector<vector<SYMBOL>> CFG = {
//	{SYMBOL::CODE, SYMBOL::VDECL, SYMBOL::CODE},
//	{SYMBOL::CODE, SYMBOL::FDECL, SYMBOL::CODE},
//	{SYMBOL::CODE, SYMBOL::EPSILON},
//
//	{SYMBOL::VDECL, SYMBOL::VTYPE, SYMBOL::ID, SYMBOL::SEMI},
//	{SYMBOL::VDECL, SYMBOL::VTYPE, SYMBOL::ASSIGN_NON, SYMBOL::SEMI},
//
//	{SYMBOL::ASSIGN_NON, SYMBOL::ID, SYMBOL::ASSIGN, SYMBOL::RHS},
//
//	{SYMBOL::FDECL, SYMBOL::VTYPE, SYMBOL::ID, SYMBOL::LPAREN, SYMBOL::ARG, SYMBOL::RPAREN, SYMBOL::LBRACE, SYMBOL::BLOCK, SYMBOL::RETURN_NON, SYMBOL::RBRACE},
//
//	{SYMBOL::ARG, SYMBOL::VTYPE, SYMBOL::ID, SYMBOL::MOREARGS},
//	{SYMBOL::ARG, SYMBOL::EPSILON},
//
//	{SYMBOL::MOREARGS, SYMBOL::COMMA, SYMBOL::VTYPE, SYMBOL::ID, SYMBOL::MOREARGS},
//	{SYMBOL::MOREARGS, SYMBOL::EPSILON},
//
//	{SYMBOL::BLOCK, SYMBOL::STMT, SYMBOL::BLOCK},
//	{SYMBOL::BLOCK, SYMBOL::EPSILON},
//
//	{SYMBOL::STMT, SYMBOL::VDECL},
//	{SYMBOL::STMT, SYMBOL::ASSIGN, SYMBOL::SEMI},
//	{SYMBOL::STMT, SYMBOL::IF, SYMBOL::LPAREN, SYMBOL::COND, SYMBOL::RPAREN, SYMBOL::LBRACE, SYMBOL::BLOCK, SYMBOL::RBRACE, SYMBOL::ELSE},
//	{SYMBOL::STMT, SYMBOL::WHILE, SYMBOL::LPAREN, SYMBOL::COND, SYMBOL::RPAREN, SYMBOL::LBRACE, SYMBOL::BLOCK, SYMBOL::RBRACE, SYMBOL::ELSE}
//};