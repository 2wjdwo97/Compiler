#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexical_analyzer.h"

//----------- 전역변수 -------------
FILE *in_fp;

int filePosition;
char lexeme[100];
int nextToken; //
bool endOfStream = false;
//----------------------------------

//----------- 함수 정의 ------------
int charToIndex(CharClass [] , char);
int charToIndex(char [], char);

int binarySearch(DfaElement dfaTable[], DfaState state);


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

bool inFinal(int final[],DfaState previousState);
bool inFinal(int final, DfaState previousState);

char getNextChar();
DfaState changeState(DfaState currentState, int inputIndex, DfaElement dfaTable[]);
//----------------------------------


/*
* Open file in w (write) mode.
* "data/file1.txt" is complete path to create file
*/


/* Write data to file */
//fputs(data, fPtr);

void main() {
	if ((in_fp = fopen("test.c", "r")) == NULL)
		printf("Error: Cannot open file 'code.c'\n");
	else {
		FILE * out_fp;
		out_fp = fopen("test.out", "w");

		/* fopen() return NULL if last operation was unsuccessful */
		if (out_fp == NULL)
		{
			printf("Unable to create file.\n");
		}

		do {
			if (isKeyword()) {
				fputs("keyword", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isVarType()) {
				fputs("varType", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isBooleanStr()) {
				fputs("boolean", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isIdentifier()) {
				fputs("id", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isBitwiseOp()) {
				fputs("bitwiseOp", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isComparisonOp()) {
				fputs("comparisonOp", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isAssignmentOp()) {
				fputs("assignment", out_fp);
				fputs("\n", out_fp);
			}
			else if (isFloatingPoint()) {
				fputs("floatingPoint", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isLiteralStr()) {
				fputs("literalStr", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isSignedInt()) {
				fputs("signedInt", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isArithmeticOp()) {
				fputs("arithmeticOp", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isComma()) {
				fputs("comma", out_fp);
				fputs("\n", out_fp);
			}
			else if (isBrace()) {
				fputs("brace", out_fp);
				fputs(" ", out_fp);
				fputs(lexeme, out_fp);
				fputs("\n", out_fp);
			}
			else if (isParen()) {
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
				fputs("whitespace", out_fp);
				fputs("\n", out_fp);
			}
		} while (nextToken != EOF);

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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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
	
	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

	while (currentState != EMPTY && !endOfStream) {
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

bool inFinal(int final[], DfaState previousState) {
	for (int i = 0; i >= sizeof(final) / sizeof(int); i++) {
		if (final[i] == previousState)
			return true;
		else
			return false;
	}
}
bool inFinal(int final, DfaState previousState) {
	if (final == previousState)
		return true;
}

char getNextChar() { // stream에서 next character 가져오는 함수
	char nextChar;

	if ((nextChar = getc(in_fp)) != EOF)
		return nextChar;
	else
		endOfStream = true;
	return NULL;
}

int charToIndex(CharClass inputList[], char inputChar) {
	
	for (int i = 0; i <= sizeof(inputList) / sizeof(int); i++) {
		if (inputList[i] == inputChar)
			return i;
		else if (inputList[i] == NON_ZERO_DIGIT) {
			if (isDigit(inputChar) && inputChar != ZERO)
				return i;
		}
		else if (inputList[i] == LETTER && isLetter(inputChar))
			return i;
	}

	return sizeof(inputList) / sizeof(int);
}

int charToIndex(char inputList[], char inputChar) {
	int i = 0;
	for (; inputList[i] != inputChar || i >= sizeof(inputList) / sizeof(char); i++);

	return i;
}

//int charToIndex(char *ALPHABET, char inputChar, DfaState *currentState) { // change 'inputChar' (character form) to 'alphabet' (integer form) and return
//	int i = 0;
//
//	if (isalpha(inputChar))
//		inputChar = "a";
//	else if (isdigit(inputChar))
//		inputChar = "0";
//
//	for (i = 0; ALPHABET[i] != inputChar || i >= sizeof(ALPHABET) / sizeof(char); i++);
//	
//	if (i >= sizeof(ALPHABET) / sizeof(char)) {
//		*currentState = EMPTY;
//		return 0;
//	}
//	else
//		return i;
//}

int binarySearch(DfaElement dfaTable[], DfaState state)
{
	int low = 0, mid, high = sizeof(dfaTable) / sizeof(DfaElement) - 1;
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

DfaState changeState(DfaState currentState, int inputIndex, DfaElement dfaTable[]) 
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
		for (int i = index; i < sizeof(dfaTable) / sizeof(DfaElement); i++) {
			if (dfaTable[i].row != currentState)
				break;
			else if (dfaTable[i].col == inputIndex)
				return dfaTable[i].value;
		}
		return EMPTY;
	}
}
