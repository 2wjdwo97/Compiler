#include "lexical_analyzer.h"

//----------------------- 전역변수 -----------------------
FILE* in_fp;

int filePosition;
char lexeme[100];
int nextToken; //
bool endOfStream = false;
//---------------------------------------------------------

//----------------------- 함수 정의 -----------------------
template <class T1, class T2>
bool DfaAccepts(T1, T2, vector<DfaState>, vector<CharClass>);

bool inFinal(const vector<DfaState> final, DfaState previousState);
bool inFinal(const DfaState final, DfaState previousState);

char getNextChar();

int charToIndex(vector<CharClass>, char);
int charToIndex(vector<char>, char);

int binarySearch(vector<DfaElement> dfaTable, DfaState state);
DfaState changeState(DfaState currentState, int inputIndex, const vector<DfaElement> dfaTable);

//---------------------------------------------------------

void main() {
	if ((in_fp = fopen("test.c", "r")) == NULL)
		printf("Error: Cannot open file 'code.c'\n");
	else {
		FILE* out_fp;
		out_fp = fopen("test.txt", "w");

		/* fopen() return NULL if last operation was unsuccessful */
		if (out_fp == NULL)
		{
			printf("Unable to create file.\n");
		}

		do {
			if (DfaAccepts(inputList_Keyword, table_Keyword, finalState_Keyword, inputList_Identifier)) {
				printf("keyword");
				fputs("keyword", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_VarType, table_VarType, finalState_VarType, inputList_Identifier)) {
				printf("varType");
				fputs("varType", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_BooleanStr, table_BooleanStr, finalState_BooleanStr, inputList_Identifier)) {
				printf("boolean");
				fputs("boolean", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_Identifier, table_Identifier, finalState_Identifier, nullVector)) {
				printf("ID");
				fputs("id", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_BitwiseOp, table_BitwiseOp, finalState_BitwiseOp, nullVector)) {
				printf("bitwiseOp");
				fputs("bitwiseOp", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_ComparisonOp, table_ComparisonOp, finalState_ComparisonOp, nullVector)) {
				printf("comparisonOp");
				fputs("comparisonOp", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_AssignmentOp, table_AssignmentOp, finalState_AssignmentOp, nullVector)) {
				printf("assignment");
				fputs("assignment", out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_FloatingPoint, table_FloatingPoint, finalState_FloatingPoint, nullVector)) {
				printf("floatingPoint");
				fputs("floatingPoint", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_LiteralStr, table_LiteralStr, finalState_LiteralStr, nullVector)) {
				printf("literalStr");
				fputs("literalStr", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_SignedInt, table_SignedInt, finalState_SignedInt, nullVector)) {
				printf("signedInt");
				fputs("signedInt", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_ArithmeticOp, table_ArithmeticOp, finalState_ArithmeticOp, nullVector)) {
				printf("arithmeticOp");
				fputs("arithmeticOp", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_Comma, table_Comma, finalState_Comma, nullVector)) {
				printf("comma");
				fputs("comma", out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_Brace, table_Brace, finalState_Brace, nullVector)) {
				printf("brace");
				fputs("brace", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_Paren, table_Paren, finalState_Paren, nullVector)) {
				printf("Paren");
				fputs("paren", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_Semicolon, table_Semicolon, finalState_Semicolon, nullVector)) {
				fputs("semicolon", out_fp);
				fputs("\n", out_fp);
			}
			else if (DfaAccepts(inputList_Whitespace, table_Whitespace, finalState_Whitespace, nullVector)) {
				printf("Whitespace");
				fputs("whitespace", out_fp);
				fputs("\n", out_fp);
			}
		} while (!endOfStream);

		fclose(out_fp);
		fclose(in_fp);
	}
}

template <class T1, class T2> // T1 char CharClass, T2 vector<vector<State>>, vector<Element>
bool DfaAccepts(T1 inputList, T2 table, vector<DfaState> finalState, vector<CharClass> condition) {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = changeState(currentState, alphabet, table); // table_Identifier[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState, previousState) && condition.size() == charToIndex(condition, currentChar)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}

bool inFinal(const vector<DfaState> final, DfaState previousState) {
	for (unsigned int i = 0; i < final.size(); i++) {
		if (final[i] == previousState)
			return true;
	}
	return false;
}
bool inFinal(const DfaState final, DfaState previousState) {
	if (final == previousState)
		return true;
	return false;
}

char getNextChar() { // stream에서 next character 가져오는 함수
	char nextChar;

	if ((nextChar = getc(in_fp)) != EOF)
		return nextChar;
	else
		endOfStream = true;
	return NULL;
}

int charToIndex(vector<CharClass> inputList, char inputChar) {

	for (unsigned int i = 0; i < inputList.size(); i++) {
		if (inputList[i] == inputChar)
			return i;
		else if (inputList[i] == NON_ZERO_DIGIT) {
			if (isdigit(inputChar) && inputChar != ZERO)
				return i;
		}
		else if (inputList[i] == LETTER && isalpha(inputChar))
			return i;
	}

	return inputList.size();
}

int charToIndex(vector<char> inputList, char inputChar) {
	unsigned int i = 0;
	for (; i < inputList.size() && inputList[i] != inputChar; i++);

	return i;
}

int binarySearch(const vector<DfaElement> dfaTable, DfaState state)
{
	int low = 0, mid, high = dfaTable.size() - 1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (state < dfaTable[mid].row)
			high = mid - 1;
		else if (state > dfaTable[mid].row)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}

DfaState changeState(DfaState currentState, int inputIndex, const vector<DfaElement> dfaTable)
{
	int index = binarySearch(dfaTable, currentState);

	if (index == -1)
		return EMPTY;
	else
	{
		for (int i = index; i >= 0; i--) {
			if (dfaTable[i].row != currentState)
				break;
			else if (dfaTable[i].col == inputIndex)
				return dfaTable[i].value;
		}
		for (unsigned int i = index; i < dfaTable.size(); i++) {
			if (dfaTable[i].row != currentState)
				break;
			else if (dfaTable[i].col == inputIndex)
				return dfaTable[i].value;
		}
		return EMPTY;
	}
}

DfaState changeState(DfaState currentState, int inputIndex, const vector<vector<DfaState>> dfaTable)
{
	return dfaTable[currentState][inputIndex];
}