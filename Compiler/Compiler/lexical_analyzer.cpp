#include "lexical_analyzer.h"

//----------------------- 전역변수 -----------------------
FILE* in_fp;

int filePosition;
char lexeme[100];
int nextToken; //
bool endOfStream = false;
//---------------------------------------------------------

//----------------------- 함수 정의 -----------------------

bool isKeyword();
bool isVarType();
bool isBooleanStr();
bool isIdentifier();

bool isBitwiseOp();
bool isComparisonOp();
bool isAssignmentOp();

bool isFloatingPoint();
bool isLiteralStr();
bool isSignedInt();

bool isArithmeticOp();
bool isComma();
bool isBrace();
bool isParen();
bool isSemicolon();
bool isWhitespace();

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
			if (isKeyword()) {
				printf("keyword");
				fputs("keyword", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isVarType()) {
				printf("varType");
				fputs("varType", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isBooleanStr()) {
				printf("boolean");
				fputs("boolean", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isIdentifier()) {
				printf("ID");
				fputs("id", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isBitwiseOp()) {
				printf("bitwiseOp");
				fputs("bitwiseOp", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isComparisonOp()) {
				printf("comparisonOp");
				fputs("comparisonOp", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isAssignmentOp()) {
				printf("assignment");
				fputs("assignment", out_fp);
				fputs("\n", out_fp);
			}
			else if (isFloatingPoint()) {
				printf("floatingPoint");
				fputs("floatingPoint", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isLiteralStr()) {
				printf("literalStr");
				fputs("literalStr", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isSignedInt()) {
				printf("signedInt");
				fputs("signedInt", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isArithmeticOp()) {
				printf("arithmeticOp");
				fputs("arithmeticOp", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isComma()) {
				printf("comma");
				fputs("comma", out_fp);
				fputs("\n", out_fp);
			}
			else if (isBrace()) {
				printf("brace");
				fputs("brace", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isParen()) {
				printf("Paren");
				fputs("paren", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isSemicolon()) {
				fputs("semicolon", out_fp);
				fputs("\n", out_fp);
			}
			else if (isWhitespace()) {
				printf("Whitespace");
				fputs("whitespace", out_fp);
				fputs("\n", out_fp);
			}
		} while (!endOfStream);

		fclose(out_fp);
		fclose(in_fp);
	}
}

bool isKeyword() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_Keyword, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = changeState(currentState, alphabet, table_Keyword); // table_Identifier[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_Keyword, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isVarType() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_VarType, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = changeState(currentState, alphabet, table_VarType); // table_Identifier[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_VarType, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isBooleanStr() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_BooleanStr, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = changeState(currentState, alphabet, table_BooleanStr); // table_Identifier[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_BooleanStr, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isIdentifier() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_Identifier, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_Identifier[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_Identifier, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isBitwiseOp() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_BitwiseOp, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_BitwiseOp[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_BitwiseOp, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isComparisonOp() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_ComparisonOp, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_ComparisonOp[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_ComparisonOp, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isAssignmentOp() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_AssignmentOp, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_AssignmentOp[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_AssignmentOp, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isFloatingPoint() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_FloatingPoint, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_FloatingPoint[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_FloatingPoint, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isLiteralStr() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_LiteralStr, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_LiteralStr[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_LiteralStr, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isSignedInt() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_SignedInt, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_SignedInt[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_SignedInt, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isArithmeticOp() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_ArithmeticOp, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_ArithmeticOp[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_ArithmeticOp, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isComma() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_Comma, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_Comma[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_Comma, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isBrace() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_Brace, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_Brace[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_Brace, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isParen() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_Paren, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_Paren[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_Paren, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isSemicolon() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_Semicolon, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_Semicolon[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_Semicolon, previousState)) { //이 전의 state가 final state인 경우
		fseek(in_fp, -1, SEEK_CUR); // file pointer move backward
		return true;
	}
	else { //getc move->read or read->move ?? i값 달라짐 상관없을 수도...
		fseek(in_fp, -i, SEEK_CUR); // file pointer move backward (next to previous token)
		return false;
	}
}
bool isWhitespace() {
	int i = 0;

	DfaState previousState;
	DfaState currentState = START_STATE;
	char currentChar;
	int alphabet;

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList_Whitespace, currentChar);

		lexeme[i++] = currentChar;

		previousState = currentState;
		currentState = table_Whitespace[currentState][alphabet]; // state transition
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme[i - 1] = '\0'; // EOS

	if (inFinal(finalState_Whitespace, previousState)) { //이 전의 state가 final state인 경우
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
