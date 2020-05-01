#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <cstdio>
#include <cctype>
#include <vector>

using namespace std;

typedef enum {
	START_STATE = 0,
	STATE_1, STATE_2, STATE_3, STATE_4, STATE_5,
	STATE_6, STATE_7, STATE_8, STATE_9, STATE_10,
	STATE_11, STATE_12, STATE_13, STATE_14, STATE_15,
	STATE_16, STATE_17, STATE_18, STATE_19, STATE_20,
	EMPTY
} DfaState;

typedef enum {
	ZERO = '0',			NON_ZERO_DIGIT = '1',	LETTER = 'a',		UNDERLINE = '_',	QUOTATION = '"',
	MINUS = '-',		PLUS = '+',				MULTIPLE = '*',		DIVIDE = '/',		EQUAL = '=',
	LESS_THAN = '<',	GREATER_THAN = '>',		EXCLAMATION = '!',	SEMICOLON = ';',	COMMA = ',',
	L_BRACE = '{',		R_BRACE = '}',			L_PAREN = '(',		R_PAREN = ')',		DOT = '.',
	AND = '&',			OR = '|',				TAB = '\t',			SPACE = ' ',		NEWLINE = '\n'
} CharClass;

vector<CharClass> inputList_SignedInt = { ZERO, MINUS, NON_ZERO_DIGIT };
vector<CharClass> inputList_LiteralStr = { QUOTATION, ZERO, NON_ZERO_DIGIT, LETTER };
vector<CharClass> inputList_FloatingPoint = { MINUS, ZERO, NON_ZERO_DIGIT, DOT };
vector<CharClass> inputList_Identifier = { UNDERLINE, ZERO, NON_ZERO_DIGIT, LETTER };
vector<CharClass> inputList_ArithmeticOp = { PLUS, EQUAL, MULTIPLE, DIVIDE };
vector<CharClass> inputList_BitwiseOp = { LESS_THAN, GREATER_THAN, AND, OR };
vector<CharClass> inputList_AssignmentOp = { EQUAL };
vector<CharClass> inputList_ComparisonOp = { LESS_THAN, GREATER_THAN, EQUAL, EXCLAMATION };
vector<CharClass> inputList_Semicolon = { SEMICOLON };
vector<CharClass> inputList_Brace = { L_BRACE, R_BRACE };
vector<CharClass> inputList_Paren = { L_PAREN, R_PAREN };
vector<CharClass> inputList_Comma = { COMMA };
vector<CharClass> inputList_Whitespace = { TAB, SPACE, NEWLINE };

//--------------------------------- Transition Table (using 2D ARRAY) ---------------------------------

const vector<DfaState> finalState_SignedInt = { STATE_1, STATE_3, STATE_4, STATE_5 };
const vector<vector<DfaState>> table_SignedInt = {
	/*							0			-			NON_ZERO_DIGIT	OTHER		// FINAL_STATE = 1,3,4,5
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3		,	EMPTY		},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	STATE_3		,	EMPTY		},
	/* STATE_3 */		{		STATE_4	,	EMPTY	,	STATE_5		,	EMPTY		},
	/* STATE_4 */		{		STATE_4	,	EMPTY	,	STATE_5		,	EMPTY		},
	/* STATE_5 */		{		STATE_4	,	EMPTY	,	STATE_5		,	EMPTY		}
};

const DfaState finalState_LiteralStr = STATE_2;
const vector<vector<DfaState>> table_LiteralStr = {
	/*							"			DIGIT		LETTER			OTHER		// FINAL_STATE = 2
	/* START_STATE */	{		STATE_1	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_1 */		{		STATE_2	,	STATE_1	,	STATE_1		,	EMPTY		},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		}
};

const vector<DfaState> finalState_FloatingPoint = { STATE_7, STATE_8 };
const vector<vector<DfaState>> table_FloatingPoint = {
	/*							-			0			NON_ZERO_DIGIT		.				OTHER		// FINAL_STATE = 7,8
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3		,		EMPTY		,	EMPTY		},
	/* STATE_1 */		{		EMPTY	,	STATE_2	,	STATE_3		,		EMPTY		,	EMPTY		},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY		,		STATE_4		,	EMPTY		},
	/* STATE_3 */		{		EMPTY	,	STATE_5	,	STATE_6		,		STATE_4		,	EMPTY		},
	/* STATE_4 */		{		EMPTY	,	STATE_7	,	STATE_8		,		EMPTY		,	EMPTY		},
	/* STATE_5 */		{		EMPTY	,	STATE_5	,	STATE_6		,		STATE_4		,	EMPTY		},
	/* STATE_6 */		{		EMPTY	,	STATE_5	,	STATE_6		,		STATE_4		,	EMPTY		},
	/* STATE_7 */		{		EMPTY	,	STATE_9	,	STATE_8		,		EMPTY		,	EMPTY		},
	/* STATE_8 */		{		EMPTY	,	STATE_9	,	STATE_8		,		EMPTY		,	EMPTY		},
	/* STATE_9 */		{		EMPTY	,	STATE_9	,	STATE_8		,		EMPTY		,	EMPTY		}
};

const vector<DfaState> finalState_Identifier = { STATE_1, STATE_2, STATE_3, STATE_4, STATE_5 };
const vector<vector<DfaState>> table_Identifier = {
	/*							_			DIGIT		LETTER			OTHER			// FINAL_STATE = 1,2,3,4,5
	/* START_STATE */	{		STATE_1	,	STATE_2	,	EMPTY		,	EMPTY		},
	/* STATE_1 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY		},
	/* STATE_2 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY		},
	/* STATE_3 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY		},
	/* STATE_4 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY		},
	/* STATE_5 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY		}
};

const vector<DfaState> finalState_ArithmeticOp = { STATE_1, STATE_2, STATE_3, STATE_4 };
const vector<vector<DfaState>> table_ArithmeticOp = {
	/*							+			=			*			/				OTHER			// FINAL_STATE = 1,2,3,4
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3	,	STATE_4		,	EMPTY		},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_3 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_4 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		}
};


const vector<DfaState> finalState_BitwiseOp = { STATE_3, STATE_4, STATE_5, STATE_6 };
const vector<vector<DfaState>> table_BitwiseOp = {
	/*							+			=			*			/				OTHER			// FINAL_STATE = 3,4,5,6
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3	,	STATE_4		,	EMPTY		},
	/* STATE_1 */		{		STATE_5	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_2 */		{		EMPTY	,	STATE_6	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_3 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_4 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_5 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_6 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		}
};

const DfaState finalState_AssignmentOp = STATE_1;
const vector<vector<DfaState>> table_AssignmentOp = {
	/*							=			OTHER						// FINAL_STATE = 1
	/* START_STATE */	{		STATE_1	,	EMPTY		},
	/* STATE_1 */		{		EMPTY	,	EMPTY		}
};

const vector<DfaState> finalState_ComparisonOp = { STATE_1, STATE_2, STATE_5, STATE_6, STATE_7, STATE_8 };
const vector<vector<DfaState>> table_ComparisonOp = {
	/*							<			>			=			!				OTHER			// FINAL_STATE = 1,2,5,6,7,8
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3	,	STATE_4		,	EMPTY		},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	STATE_5	,	EMPTY		,	EMPTY		},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	STATE_6	,	EMPTY		,	EMPTY		},
	/* STATE_3 */		{		EMPTY	,	EMPTY	,	STATE_7	,	EMPTY		,	EMPTY		},
	/* STATE_4 */		{		EMPTY	,	EMPTY	,	STATE_8	,	EMPTY		,	EMPTY		},
	/* STATE_5 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_6 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_7 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		},
	/* STATE_8 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY		}
};

const DfaState finalState_Semicolon = STATE_1;
const vector<vector<DfaState>> table_Semicolon = {
	/*							;			OTHER						// FINAL_STATE = 1
	/* START_STATE */	{		STATE_1	,	EMPTY		},
	/* STATE_1 */		{		EMPTY	,	EMPTY		}
};

const vector<DfaState> finalState_Brace = { STATE_1, STATE_2 };
const vector<vector<DfaState>> table_Brace = {
	/*							{			}			OTHER			// FINAL_STATE = 1,2
	/* START_STATE */	{		STATE_1	,	STATE_2	,	EMPTY		},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	EMPTY		},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY		}
};

const vector<DfaState> finalState_Paren = { STATE_1, STATE_2 };
const vector<vector<DfaState>> table_Paren = {
	/*							(			)			OTHER			// FINAL_STATE = 1,2
	/* START_STATE */	{		STATE_1	,	STATE_2	,	EMPTY		},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	EMPTY		},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY		}
};

const DfaState finalState_Comma = STATE_1;
const vector<vector<DfaState>> table_Comma = {
	/*							;			OTHER			// FINAL_STATE = 1
	/* START_STATE */	{		STATE_1	,	EMPTY	},
	/* STATE_1 */		{		EMPTY	,	EMPTY	}
};

const DfaState finalState_Whitespace = STATE_1;
const vector<vector<DfaState>> table_Whitespace = {
	/*							tab			space		newline		OTHER		// FINAL_STATE = 1
	/* START_STATE */	{		STATE_1	,	STATE_1	,	STATE_1	,	EMPTY		},
	/* STATE_1 */		{		STATE_1	,	STATE_1	,	STATE_1	,	EMPTY		}
};
//----------------------------------------------------------------------------------------------


//--------------------------- Transition Table (using SPARSE MATRIX) ---------------------------
typedef struct {
	int row;
	int col;
	DfaState value;
} DfaElement;

vector<char> inputList_VarType = { 'a', 'b', 'c', 'f', 'h', 'i', 'l', 'n', 'o', 'r', 't' };
vector<char> inputList_BooleanStr = { 'a', 'e', 'f', 'l', 'r', 's', 't', 'u' };
vector<char> inputList_Keyword = { 'e', 'f', 'h', 'i', 'l', 'n', 'o', 'r', 's', 't', 'u', 'w' };

const vector<DfaState> finalState_VarType = { STATE_9, STATE_13, STATE_14, STATE_16 };
const vector<DfaElement> table_VarType = {
	//	row				col		value
	{	START_STATE	,	1	,	STATE_3		},
	{	START_STATE	,	2	,	STATE_2		},
	{	START_STATE	,	3	,	STATE_4		},
	{	START_STATE	,	5	,	STATE_1		},
	{	STATE_1		,	7	,	STATE_5		},
	{	STATE_2		,	4	,	STATE_6		},
	{	STATE_3		,	8	,	STATE_7		},
	{	STATE_4		,	6	,	STATE_8		},
	{	STATE_5		,	10	,	STATE_9		},
	{	STATE_6		,	0	,	STATE_10	},
	{	STATE_7		,	8	,	STATE_11	},
	{	STATE_8		,	8	,	STATE_12	},
	{	STATE_10	,	9	,	STATE_13	},
	{	STATE_11	,	6	,	STATE_14	},
	{	STATE_12	,	0	,	STATE_15	},
	{	STATE_15	,	10	,	STATE_16	}
};// FINAL_STATE = 9, 13, 14, 16

const vector<DfaState> finalState_BooleanStr = { STATE_7, STATE_9 };
const vector<DfaElement> table_BooleanStr = {
	//	row				col		value
	{	START_STATE	,	2	,	STATE_2		},
	{	START_STATE	,	6	,	STATE_1		},
	{	STATE_1		,	4	,	STATE_3		},
	{	STATE_2		,	0	,	STATE_4		},
	{	STATE_3		,	7	,	STATE_5		},
	{	STATE_4		,	3	,	STATE_6		},
	{	STATE_5		,	1	,	STATE_7		},
	{	STATE_6		,	5	,	STATE_8		},
	{	STATE_8		,	1	,	STATE_9		}
};// FINAL_STATE = 7, 9

const vector<DfaState> finalState_Keyword = { STATE_6, STATE_13, STATE_15, STATE_18, STATE_20 };
const vector<DfaElement> table_Keyword = {
	//	row				col		value
	{	START_STATE	,	0	,	STATE_2		},
	{	START_STATE	,	1	,	STATE_4		},
	{	START_STATE	,	3	,	STATE_1		},
	{	START_STATE	,	7	,	STATE_5		},
	{	START_STATE	,	11	,	STATE_3		},
	{	STATE_1	,		1	,	STATE_6		},
	{	STATE_2	,		4	,	STATE_7		},
	{	STATE_3	,		2	,	STATE_8		},
	{	STATE_4	,		6	,	STATE_9		},
	{	STATE_5	,		0	,	STATE_10	},
	{	STATE_7	,		8	,	STATE_11	},
	{	STATE_8	,		3	,	STATE_12	},
	{	STATE_9	,		7	,	STATE_13	},
	{	STATE_10	,	9	,	STATE_14	},
	{	STATE_11	,	0	,	STATE_15	},
	{	STATE_12	,	4	,	STATE_16	},
	{	STATE_14	,	10	,	STATE_17	},
	{	STATE_16	,	0	,	STATE_18	},
	{	STATE_17	,	7	,	STATE_19	},
	{	STATE_19	,	5	,	STATE_20	}
}; // FINAL_STATE = 6, 13, 15, 18, 20
