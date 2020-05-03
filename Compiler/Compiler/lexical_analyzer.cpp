#include "lexical_analyzer.h"

//----------------------- 전역변수 -----------------------
//FILE* in_fp;
ifstream readFile("test.c");

int filePosition;
char lexeme[100];
bool endOfStream = false;
bool isPreviousTokenOperand = false;
//---------------------------------------------------------

//----------------------- 함수 정의 -----------------------
void countNewLine(int *);
template <class T1, class T2, class T3>
bool DfaAccepts(const T1, const T2, const vector<DfaState>, T3);

bool meetCondition(const vector<CharClass>, char);
bool meetCondition(bool, char);

bool inFinal(const vector<DfaState> final, DfaState previousState);

char getNextChar();

int charToIndex(const vector<CharClass>, char);
int charToIndex(const vector<char>, char);

int binarySearch(const vector<DfaElement> dfaTable, const DfaState state);
DfaState changeState(const DfaState currentState, int inputIndex, const vector<DfaElement> dfaTable);
DfaState changeState(const DfaState currentState, int inputIndex, const vector<vector<DfaState>> dfaTable);

//---------------------------------------------------------

void main() {
	int currentLine = 1;
	bool ErrorFound;

	if (!readFile.is_open())
	{
		printf("Error: Cannot open file 'code.c'\n");
		exit(0);
	}		
	else {
		ofstream writeFile("test.txt");

		do {
			ErrorFound = true;

			if (DfaAccepts(inputList_Keyword, table_Keyword, finalState_Keyword, inputList_Identifier)) {
				printf("Keyword");
				isPreviousTokenOperand = false;
				writeFile << "Keyword" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_VarType, table_VarType, finalState_VarType, inputList_Identifier)) {
				printf("VarType");
				isPreviousTokenOperand = false;
				writeFile << "VarType" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_BooleanStr, table_BooleanStr, finalState_BooleanStr, inputList_Identifier)) {
				printf("Boolean");
				isPreviousTokenOperand = false;
				writeFile << "Boolean" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_Identifier, table_Identifier, finalState_Identifier, nullVector)) {
				printf("Identifier");
				isPreviousTokenOperand = true;
				writeFile << "Identifier" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_BitwiseOp, table_BitwiseOp, finalState_BitwiseOp, nullVector)) {
				printf("BitwiseOp");
				isPreviousTokenOperand = false;
				writeFile << "BitwiseOp" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_ComparisonOp, table_ComparisonOp, finalState_ComparisonOp, nullVector)) {
				printf("ComparisonOp");
				isPreviousTokenOperand = false;
				writeFile << "ComparisonOp" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_AssignmentOp, table_AssignmentOp, finalState_AssignmentOp, nullVector)) {
				printf("Assignment");
				isPreviousTokenOperand = false;
				writeFile << "Assignment" << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_FloatingPoint, table_FloatingPoint, finalState_FloatingPoint, isPreviousTokenOperand)) {
				printf("FloatingPoint");
				isPreviousTokenOperand = true;
				writeFile << "FloatingPoint" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_LiteralStr, table_LiteralStr, finalState_LiteralStr, nullVector)) {
				printf("literalStr");
				isPreviousTokenOperand = false;
				writeFile << "literalStr" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_SignedInt, table_SignedInt, finalState_SignedInt, isPreviousTokenOperand)) {
				printf("SignedInt");
				isPreviousTokenOperand = true;
				writeFile << "SignedInt" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_ArithmeticOp, table_ArithmeticOp, finalState_ArithmeticOp, nullVector)) {
				printf("ArithmeticOp");
				isPreviousTokenOperand = false;
				writeFile << "ArithmeticOp" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_Comma, table_Comma, finalState_Comma, nullVector)) {
				printf("Comma");
				isPreviousTokenOperand = false;
				writeFile << "Comma" << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_Brace, table_Brace, finalState_Brace, nullVector)) {
				printf("Brace");
				isPreviousTokenOperand = false;
				writeFile << "Brace" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_Paren, table_Paren, finalState_Paren, nullVector)) {
				printf("Paren");
				if (lexeme[0] == ')')
					isPreviousTokenOperand = true;
				else
					isPreviousTokenOperand = false;
				writeFile << "Paren" << " " << lexeme << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_Semicolon, table_Semicolon, finalState_Semicolon, nullVector)) {
				printf("Semicolon");
				isPreviousTokenOperand = false;
				writeFile << "Semicolon" << endl;
				ErrorFound = false;
			}
			else if (DfaAccepts(inputList_Whitespace, table_Whitespace, finalState_Whitespace, nullVector)) {
				printf("Whitespace");
				writeFile << "Whitespace" << endl;
				countNewLine(&currentLine);
				ErrorFound = false;
			}
		} while (!endOfStream && !ErrorFound);

		if (ErrorFound)
			printf("Error: line - %d", currentLine);

		writeFile.close();
		readFile.close();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T1, class T2, class T3> // T1 char CharClass, T2 vector<vector<State>>, vector<Element>
bool DfaAccepts(const T1 inputList, const T2 table, const vector<DfaState> finalState, T3 condition) {
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
	
	if (!currentChar)
		i--;

	if (inFinal(finalState, previousState) && meetCondition(condition, currentChar)) { //이 전의 state가 final state인 경우
		readFile.seekg(-1, ios::cur); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		readFile.seekg(-i, ios::cur); // file pointer move backward (next to previous token)
		endOfStream = false;
		return false;
	}
}

bool meetCondition(const vector<CharClass> condition, char currentChar) {
	if (condition.size() == charToIndex(condition, currentChar))
		return true;
	else
		return false;
}
//SignedInteger, FloatingPoint 검사 : 이전 토큰이 { R_PAREN, ZERO, NON_ZERO_DIGIT, LETTER } 인 경우
bool meetCondition(bool isPreviousTokenOperand, char currentChar) {
	if (!isPreviousTokenOperand || lexeme[0] != '-')
		return true;
	else
		return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool inFinal(const vector<DfaState> final, const DfaState previousState) {
	for (unsigned int i = 0; i < final.size(); i++) {
		if (final[i] == previousState)
			return true;
	}
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

int charToIndex(const vector<CharClass> inputList, char inputChar) {

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

int charToIndex(const vector<char> inputList, char inputChar) {
	unsigned int i = 0;
	for (; i < inputList.size() && inputList[i] != inputChar; i++);

	return i;
}

int binarySearch(const vector<DfaElement> dfaTable, const DfaState state)
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

DfaState changeState(const DfaState currentState, int inputIndex, const vector<DfaElement> dfaTable)
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

DfaState changeState(const DfaState currentState, int inputIndex, const vector<vector<DfaState>> dfaTable)
{
	return dfaTable[currentState][inputIndex];
}

void countNewLine(int *currentLine) {
	int i;
	for (i = 0; lexeme[i] != '\0'; i++) {
		if(lexeme[i] == '\n')
			*currentLine = *currentLine + 1;
	}
}