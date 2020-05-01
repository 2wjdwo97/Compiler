#pragma once

typedef enum {
	START_STATE = 0,
	STATE_1,	STATE_2,	STATE_3,	STATE_4,	STATE_5,
	STATE_6,	STATE_7,	STATE_8,	STATE_9,	STATE_10,
	STATE_11,	STATE_12,	STATE_13,	STATE_14,	STATE_15,
	STATE_16,	STATE_17,	STATE_18,	STATE_19,	STATE_20,
	EMPTY
} DfaState;

typedef enum {
	ZERO = '0',			NON_ZERO_DIGIT = '1',	LETTER = 'a',		UNDERLINE = '_',		QUOTATION = '"',
	MINUS = '-',		PLUS = '+',				MULTIPLE = '*',		DIVIDE = '/',			EQUAL = '=',
	LESS_THAN = '<',	GREATER_THAN = '>',		EXCLAMATION = '!',	SEMICOLON = ';',		COMMA = ',',
	L_BRACE = '{',		R_BRACE = '}',			L_PAREN = '(',		R_PAREN = ')',			DOT = '.',
	AND = '&',			OR = '|'
}CharClass;

CharClass inputList_SignedInt[3] = { ZERO, MINUS, NON_ZERO_DIGIT };
CharClass inputList_LiteralStr[4] = { QUOTATION, ZERO, NON_ZERO_DIGIT, LETTER };
CharClass inputList_FloatingPoint[4] = { MINUS, ZERO, NON_ZERO_DIGIT, DOT };
CharClass inputList_Identifier[4] = { UNDERLINE, ZERO, NON_ZERO_DIGIT, LETTER };
CharClass inputList_ArithmeticOp[4] = { PLUS, EQUAL, MULTIPLE, DIVIDE };
CharClass inputList_BitwiseOp[4] = { LESS_THAN, GREATER_THAN, AND, OR };
CharClass inputList_AssignmentOp[1] = { EQUAL };
CharClass inputList_ComparisonOp[4] = { LESS_THAN, GREATER_THAN, EQUAL, EXCLAMATION };
CharClass inputList_Semicolon[1] = { SEMICOLON };
CharClass inputList_Brace[2] = { L_BRACE, R_BRACE };
CharClass inputList_Paren[2] = { L_PAREN, R_PAREN };
CharClass inputList_Comma[1] = { COMMA };

//--------------------------------- Transition Table (using 2D ARRAY) ---------------------------------

const int finalState_SignedInt[] = { 1, 3, 4,5 };
const DfaState table_SignedInt[6][4] = {
	/*							0			-			NON_ZERO_DIGIT	*/	// FINAL_STATE = 1,3,4,5
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3		,	EMPTY	},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	STATE_3		,	EMPTY	},
	/* STATE_3 */		{		STATE_4	,	EMPTY	,	STATE_5		,	EMPTY	},
	/* STATE_4 */		{		STATE_4	,	EMPTY	,	STATE_5		,	EMPTY	},
	/* STATE_5 */		{		STATE_4	,	EMPTY	,	STATE_5		,	EMPTY	}
};

const int finalState_LiteralStr = 2;
const DfaState table_LiteralStr[3][4] = {
	/*							"			DIGIT		LETTER		*/	// FINAL_STATE = 2
	/* START_STATE */	{		STATE_1	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_1 */		{		STATE_2	,	STATE_1	,	STATE_1		,	EMPTY	},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	}
};

const int finalState_FloatingPoint[] = { 7, 8 };
const DfaState table_FloatingPoint[10][5] = {
	/*							-			0			NON_ZERO_DIGIT		.		*/	// FINAL_STATE = 7,8
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3		,	EMPTY		,	EMPTY	},
	/* STATE_1 */		{		EMPTY	,	STATE_2	,	STATE_3		,	EMPTY		,	EMPTY	},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY		,	STATE_4		,	EMPTY	},
	/* STATE_3 */		{		EMPTY	,	STATE_5	,	STATE_6		,	STATE_4		,	EMPTY	},
	/* STATE_4 */		{		EMPTY	,	STATE_7	,	STATE_8		,	EMPTY		,	EMPTY	},
	/* STATE_5 */		{		EMPTY	,	STATE_5	,	STATE_6		,	STATE_4		,	EMPTY	},
	/* STATE_6 */		{		EMPTY	,	STATE_5	,	STATE_6		,	STATE_4		,	EMPTY	},
	/* STATE_7 */		{		EMPTY	,	STATE_9	,	STATE_8		,	EMPTY		,	EMPTY	},
	/* STATE_8 */		{		EMPTY	,	STATE_9	,	STATE_8		,	EMPTY		,	EMPTY	},
	/* STATE_9 */		{		EMPTY	,	STATE_9	,	STATE_8		,	EMPTY		,	EMPTY	}
};

const int finalState_Identifier[] = { 1, 2, 3, 4, 5};
const DfaState table_Identifier[6][4] = {
	/*							_			DIGIT		LETTER			*/	// FINAL_STATE = 1,2,3,4,5
	/* START_STATE */	{		STATE_1	,	STATE_2	,	EMPTY		,	EMPTY	},
	/* STATE_1 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY	},
	/* STATE_2 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY	},
	/* STATE_3 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY	},
	/* STATE_4 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY	},
	/* STATE_5 */		{		STATE_3	,	STATE_4	,	STATE_5		,	EMPTY	}
};

const int finalState_ArithmeticOp[] = { 1, 2, 3, 4 };
const DfaState table_ArithmeticOp[5][4] = {
	/*							+			=			*			/			*/	// FINAL_STATE = 1,2,3,4
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3	,	STATE_4		,	EMPTY	},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_3 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_4 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	}
};


const int finalState_BitwiseOp[] = { 3, 4, 5, 6 };
const DfaState table_BitwiseOp[7][5] = {
	/*							+			=			*			/			*/	// FINAL_STATE = 3,4,5,6
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3	,	STATE_4		,	EMPTY	},
	/* STATE_1 */		{		STATE_5	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_2 */		{		EMPTY	,	STATE_6	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_3 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_4 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_5 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_6 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	}
};

const int finalState_AssignmentOp = 1;
const DfaState table_AssignmentOp[2][2] = {
	/*							=			*/	// FINAL_STATE = 1
	/* START_STATE */	{		STATE_1	,	EMPTY },
	/* STATE_1 */		{		EMPTY	,	EMPTY }
};

const int finalState_ComparisonOp[] = { 1, 2, 5, 6, 7, 8 };
const DfaState table_ComparisonOp[9][5] = {
	/*							<			>			=			!			*/	// FINAL_STATE = 1,2,5,6,7,8
	/* START_STATE */	{		STATE_1	,	STATE_2	,	STATE_3	,	STATE_4		,	EMPTY	},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	STATE_5	,	EMPTY		,	EMPTY	},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	STATE_6	,	EMPTY		,	EMPTY	},
	/* STATE_3 */		{		EMPTY	,	EMPTY	,	STATE_7	,	EMPTY		,	EMPTY	},
	/* STATE_4 */		{		EMPTY	,	EMPTY	,	STATE_8	,	EMPTY		,	EMPTY	},
	/* STATE_5 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_6 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_7 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	},
	/* STATE_8 */		{		EMPTY	,	EMPTY	,	EMPTY	,	EMPTY		,	EMPTY	}
};

const int finalState_Semicolon = 1;
const DfaState table_Semicolon[2][2] = {
	/*							;			*/	// FINAL_STATE = 1
	/* START_STATE */	{		STATE_1	,	EMPTY	},
	/* STATE_1 */		{		EMPTY	,	EMPTY	}
};

const int finalState_Brace[] = { 1, 2 };
const DfaState table_Brace[3][3] = {
	/*							{			}			*/	// FINAL_STATE = 1,2
	/* START_STATE */	{		STATE_1	,	STATE_2	,	EMPTY	},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	EMPTY	},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY	}
};

const int finalState_Paren[] = { 1, 2 };
const DfaState table_Paren[3][3] = {
	/*							(			)			*/	// FINAL_STATE = 1,2
	/* START_STATE */	{		STATE_1	,	STATE_2	,	EMPTY	},
	/* STATE_1 */		{		EMPTY	,	EMPTY	,	EMPTY	},
	/* STATE_2 */		{		EMPTY	,	EMPTY	,	EMPTY	}
};

const int finalState_Comma = 1;
const DfaState table_Comma[2][2] = {
	/*							;			*/	// FINAL_STATE = 1
	/* START_STATE */	{		STATE_1	,	EMPTY	},
	/* STATE_1 */		{		EMPTY	,	EMPTY	}
};
//----------------------------------------------------------------------------------------------


//--------------------------- Transition Table (using SPARSE MATRIX) ---------------------------
typedef struct {
	int row;
	int col;
	int value;
} DfaElement;

char inputList_VarType[11] = { 'a', 'b', 'c', 'f', 'h', 'i', 'l', 'n', 'o', 'r', 't' };
char inputList_BooleanStr[8] = { 'a', 'e', 'f', 'l', 'r', 's', 't', 'u' };
char inputList_Keyword[12] = { 'e', 'f', 'h', 'i', 'l', 'n', 'o', 'r', 's', 't', 'u', 'w' };

const int finalState_VarType[] = { 9, 13, 14, 16 };
const DfaElement table_VarType[16] = {
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

const int finalState_BooleanStr[] = { 7, 9 };
const DfaElement table_BooleanStr[9] = {
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

const int finalState_Keyword[] = { 6, 13, 15, 18, 20 };
const DfaElement table_Keyword[20] = {
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
