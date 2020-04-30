#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexical_analyzer.h"

//----------- 전역변수 -------------
FILE *in_fp, *fopen();
bool isKeyword();
bool isType();
bool isIdentifier();

char nextChar; // ?
int filePosition;
char lexeme[100];
int lexLen;
int token;
int nextToken;
bool endOfStream = false;
//----------------------------------

//----------- 함수 정의 ------------
int charToIndex(char *alphabet,char inputChar, DfaState *currentState);
int binarySearch(int a[], int keyValue);
bool isKeyword();
bool isType();
bool isIdentifier();
bool isInteger();
bool isFloatingPoint();

//----------------------------------

void main() {
	if ((in_fp = fopen("code.c", "r")) == NULL)
		printf("Error: Cannot open file 'code.c'\n");
	else {
		do {
			if (isKeyword()) {

			}
			else if (isType()) {

			}
			else if (isIdentifier()) {

			}
		} while (nextToken != EOF);
	}
}

bool isKeyword() {
	DfaState currentState = START_STATE;
	char currentChar = getNextChar();

	int inputChar = charToIndex(currentChar, &currentChar);
	
	while (currentState != EMPTY && !endOfStream) {
		currentState = table_Keyword[currentState][]; // currentChar를 char -> int 함수에 넣기
		currentChar = getNextChar();
	}
}

bool isType() {
	DfaState currentState = START_STATE;
	char currentChar = getNextChar();
	const char* TYPE_ALPHABET = "a";

	int inputChar = charToIndex(TYPE_ALPHABET, currentChar, &currentChar);

	while (currentState != EMPTY && !endOfStream) {
		currentState = table_Type[currentState][]; // currentChar를 char -> int 함수에 넣기
		currentChar = getNextChar();
	}
}

bool isInterger() {
	DfaState currentState = START_STATE;
	char currentChar = getNextChar();
	const char* INTEGER_ALPHABET = "0-1"; // 0: 0, 1: Non Zero Digit

	int inputChar = charToIndex(INTEGER_ALPHABET, currentChar, &currentChar);

	while (currentState != EMPTY && !endOfStream) {
		currentState = table_SignedInt[currentState][]; // currentChar를 char -> int 함수에 넣기
		currentChar = getNextChar();
	}
}

bool isFloatingPoint() {
	DfaState currentState = START_STATE;
	char currentChar = getNextChar();
	const char* TYPE_ALPHABET = "a";

	int inputChar = charToIndex(TYPE_ALPHABET, currentChar, &currentChar);

	while (currentState != EMPTY && !endOfStream) {
		currentState = table_Type[currentState][]; // currentChar를 char -> int 함수에 넣기
		currentChar = getNextChar();
	}
}

bool isIdentifier() {
	const char ID_ALPHABET[] = "_0a"; // 0 :digit, a : letter

	DfaState currentState = START_STATE;
	char currentChar = getNextChar();
	int alphabet = charToIndex(ID_ALPHABET, currentChar, &currentState);

	while (currentState != EMPTY && !endOfStream) {
		currentState = table_Identifier[currentState][alphabet]; // state transition
		
		currentChar = getNextChar();
		alphabet = charToIndex(ID_ALPHABET, currentChar, &currentState);  
	}
}

char getNextChar() { // stream에서 next character 가져오는 함수
	char nextChar;

	if ((nextChar = getc(in_fp)) != EOF)
		return nextChar;
	else
		endOfStream = true;
	return NULL;
}

int charToIndex(char *ALPHABET, char inputChar, DfaState *currentState) { // change 'inputChar' (character form) to 'alphabet' (integer form) and return
	int i = 0;

	if (isalpha(inputChar))
		inputChar = "a";
	else if (isdigit(inputChar))
		inputChar = "0";

	for (i = 0; ALPHABET[i] != inputChar || i >= sizeof(ALPHABET) / sizeof(char); i++);
	
	if (i >= sizeof(ALPHABET) / sizeof(char)) {
		*currentState = EMPTY;
		return 0;
	}
	else
		return i;
}

int binarySearch(int a[], int keyValue)
{
	int low = 0, mid, high = sizeof(a) / sizeof(int) - 1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (keyValue < a[mid])
			high = mid - 1;
		else if (keyValue > a[mid])
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}
