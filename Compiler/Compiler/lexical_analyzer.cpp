#include "lexical_analyzer.h"

//----------------------- 전역변수 -----------------------
//FILE* in_fp;
ifstream readFile("test.c");

int filePosition;
char lexeme[100];
int nextToken; //
bool endOfStream = false;
bool isPreviousTokenOperand = false;
//---------------------------------------------------------

//----------------------- 함수 정의 -----------------------

template <class T1, class T2, class T3>
bool DfaAccepts(T1, T2, vector<DfaState>, T3);

bool meetCondition(vector<CharClass>, char);
bool meetCondition(bool, char);
void isOperand(char);

bool inFinal(const vector<DfaState> final, DfaState previousState);
bool inFinal(const DfaState final, DfaState previousState);

char getNextChar();

int charToIndex(vector<CharClass>, char);
int charToIndex(vector<char>, char);

int binarySearch(vector<DfaElement> dfaTable, DfaState state);
DfaState changeState(DfaState currentState, int inputIndex, const vector<DfaElement> dfaTable);

//---------------------------------------------------------

void main() {
	
	if (!readFile.is_open())
	{
		printf("Error: Cannot open file 'code.c'\n");
		exit(0);
	}		
	else {
		ofstream writeFile("test.txt");

		do {
			if (DfaAccepts(inputList_Keyword, table_Keyword, finalState_Keyword, inputList_Identifier)) {
				printf("Keyword");
				writeFile << "Keyword" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_VarType, table_VarType, finalState_VarType, inputList_Identifier)) {
				printf("VarType");
				writeFile << "VarType" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_BooleanStr, table_BooleanStr, finalState_BooleanStr, inputList_Identifier)) {
				printf("Boolean");
				writeFile << "Boolean" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_Identifier, table_Identifier, finalState_Identifier, nullVector)) {
				printf("Identifier");
				writeFile << "Identifier" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_BitwiseOp, table_BitwiseOp, finalState_BitwiseOp, nullVector)) {
				printf("BitwiseOp");
				writeFile << "BitwiseOp" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_VarType, table_VarType, finalState_VarType, inputList_Identifier)) {
				printf("ComparisonOp");
				writeFile << "ComparisonOp" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_AssignmentOp, table_AssignmentOp, finalState_AssignmentOp, nullVector)) {
				printf("Assignment");
				writeFile << "Assignment" << endl;
			}
			else if (DfaAccepts(inputList_FloatingPoint, table_FloatingPoint, finalState_FloatingPoint, nullVector)) {
				printf("FloatingPoint");
				writeFile << "FloatingPoint" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_LiteralStr, table_LiteralStr, finalState_LiteralStr, nullVector)) {
				printf("literalStr");
				writeFile << "literalStr" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_SignedInt, table_SignedInt, finalState_SignedInt, nullVector)) {
				printf("SignedInt");
				writeFile << "SignedInt" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_ArithmeticOp, table_ArithmeticOp, finalState_ArithmeticOp, nullVector)) {
				printf("ArithmeticOp");
				writeFile << "ArithmeticOp" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_Comma, table_Comma, finalState_Comma, nullVector)) {
				printf("Comma");
				writeFile << "Comma" << endl;
			}
			else if (DfaAccepts(inputList_Brace, table_Brace, finalState_Brace, nullVector)) {
				printf("Brace");
				writeFile << "Brace" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_Paren, table_Paren, finalState_Paren, nullVector)) {
				printf("Paren");
				writeFile << "Paren" << " " << lexeme << endl;
			}
			else if (DfaAccepts(inputList_Semicolon, table_Semicolon, finalState_Semicolon, nullVector)) {
				printf("Semicolon");
				writeFile << "Semicolon" << endl;
			}
			else if (DfaAccepts(inputList_Whitespace, table_Whitespace, finalState_Whitespace, nullVector)) {
				printf("Whitespace");
				writeFile << "Whitespace" << endl;
			}
		} while (!endOfStream);

		writeFile.close();
		readFile.close();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T1, class T2, class T3> // T1 char CharClass, T2 vector<vector<State>>, vector<Element>
bool DfaAccepts(T1 inputList, T2 table, vector<DfaState> finalState, T3 condition) {
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

	if (inFinal(finalState, previousState) && meetCondition(condition, currentChar)) { //이 전의 state가 final state인 경우
		isOperand(currentChar);
		readFile.seekg(-1, ios::cur); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		readFile.seekg(-i, ios::cur);; // file pointer move backward (next to previous token)
		return false;
	}
}

bool meetCondition(vector<CharClass> condition, char currentChar) {
	if (condition.size() == charToIndex(condition, currentChar))
		return true;
	else
		return false;
}
//SignedInteger, FloatingPoint 검사 : 이전 토큰이 { R_PAREN, ZERO, NON_ZERO_DIGIT, LETTER } 인 경우
bool meetCondition(bool isPreviousTokenOperand, char currentChar) {
	if (!isPreviousTokenOperand)
		return true;
	else
		return false;
}

void isOperand(char currentChar) {
	if (inputList_isPreviousTokenOperand.size() == charToIndex(inputList_isPreviousTokenOperand, currentChar))
		isPreviousTokenOperand = false;
	else
		isPreviousTokenOperand = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	if ((nextChar = readFile.get()) != EOF)
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