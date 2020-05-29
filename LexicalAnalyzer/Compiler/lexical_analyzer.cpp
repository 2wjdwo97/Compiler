#include "lexical_analyzer.h"

//----------------------- 전역변수 -----------------------
ifstream readFile;

int readPosition = 0;
int currentLine = 1;
vector<char> lexeme;
bool isPreviousTokenOperand = false;

string code;

MaxLengthToken maxLengthToken;
vector<ErrorData> errorData;
//---------------------------------------------------------

//----------------------- 함수 선언 -----------------------
void WriteToken(ofstream* writeFile);

template <class T1, class T2>
void DfaAccepts(const T1, const T2, const vector<DfaState>, TokenName);

bool meetCondition(TokenName, char);

bool inFinal(const vector<DfaState> final, DfaState previousState);

char getNextChar();
int charToIndex(const vector<CharClass>, char);
int charToIndex(const vector<char>, char);

int binarySearch(const vector<DfaElement> dfaTable, const DfaState state);
DfaState changeState(const DfaState currentState, int inputIndex, const vector<DfaElement> dfaTable);
DfaState changeState(const DfaState currentState, int inputIndex, const vector<vector<DfaState>> dfaTable);

void countNewLine();
//---------------------------------------------------------

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "The execution command of the lexical analyzer: lexical_analyzer <input_file_name> \n";
		exit(0);
	}

	readFile.open(argv[1]);

	if (!readFile.is_open())
	{
		cout << "Error: Cannot open file : " << argv[1];
		exit(0);
	}
	else {
		code.assign((std::istreambuf_iterator<char>(readFile)),
			(std::istreambuf_iterator<char>()));
		code.push_back(EOF);
		string fileName;
		
		for (int i = 0; argv[1][i] != '.'; i++)
			fileName.push_back(argv[1][i]);

		ofstream writeFile;
		writeFile.open(fileName + ".out");
		while (code[readPosition] != EOF) { //EOF가 아닐 때 반복
			// DfaAccepts 함수를 각 토큰의 Dfa에 대해서 차례로 호출
			DfaAccepts(inputList_Keyword, table_Keyword, finalState_Keyword, Keyword);
			DfaAccepts(inputList_VarType, table_VarType, finalState_VarType, VarType);
			DfaAccepts(inputList_BooleanStr, table_BooleanStr, finalState_BooleanStr, BooleanStr);
			DfaAccepts(inputList_Identifier, table_Identifier, finalState_Identifier, Identifier);
			DfaAccepts(inputList_BitwiseOp, table_BitwiseOp, finalState_BitwiseOp, BitwiseOp);
			DfaAccepts(inputList_ComparisonOp, table_ComparisonOp, finalState_ComparisonOp, ComparisonOp);
			DfaAccepts(inputList_AssignmentOp, table_AssignmentOp, finalState_AssignmentOp, AssignmentOp);
			DfaAccepts(inputList_FloatingPoint, table_FloatingPoint, finalState_FloatingPoint, FloatingPoint);
			DfaAccepts(inputList_LiteralStr, table_LiteralStr, finalState_LiteralStr, LiteralStr);
			DfaAccepts(inputList_SignedInt, table_SignedInt, finalState_SignedInt, SignedInt);
			DfaAccepts(inputList_ArithmeticOp, table_ArithmeticOp, finalState_ArithmeticOp, ArithmeticOp);
			DfaAccepts(inputList_Comma, table_Comma, finalState_Comma, Comma);
			DfaAccepts(inputList_Brace, table_Brace, finalState_Brace, Brace);
			DfaAccepts(inputList_Paren, table_Paren, finalState_Paren, Paren);
			DfaAccepts(inputList_Semicolon, table_Semicolon, finalState_Semicolon, Semicolon);
			DfaAccepts(inputList_Whitespace, table_Whitespace, finalState_Whitespace, Whitespace);
			
			if (maxLengthToken.maxLength == 0) { // accept된 dfa가 하나도 없다면 error로 판단
				ErrorData newError;
				newError.line = currentLine;
				newError.wrongInput = code[readPosition++];
				errorData.push_back(newError);
			}
			else { // accept된 dfa가 있다면 그 token의 정보를 파일에 저장
				WriteToken(&writeFile);
				readPosition += maxLengthToken.maxLength;
			}

			// maxLengthToken의 변수들을 초기화
			maxLengthToken.maxLength = 0;
			maxLengthToken.tokenName = Null;
			maxLengthToken.tokenValue.clear();
		}

		// Error 출력

		for (unsigned int i = 0; i < errorData.size(); i++)
			printf("Error Line: %d, Wrong Input: %c\n", errorData[i].line, errorData[i].wrongInput);

		writeFile.close();
		readFile.close();
	}
}

void WriteToken(ofstream *writeFile) {
	// file write할 때 tokenName에 따라 각 동작을 수행
	switch (maxLengthToken.tokenName) {
	case Keyword:
		isPreviousTokenOperand = false;
		*writeFile << "KEYWORD" << " " << maxLengthToken.tokenValue << endl;
		break;
	case VarType:
		isPreviousTokenOperand = false;
		*writeFile << "VTYPE" << " " << maxLengthToken.tokenValue << endl;
		break;
	case BooleanStr:
		isPreviousTokenOperand = false;
		*writeFile << "BOOL" << " " << maxLengthToken.tokenValue << endl;
		break;
	case Identifier:
		isPreviousTokenOperand = true;
		*writeFile << "ID" << " " << maxLengthToken.tokenValue << endl;
		break;
	case BitwiseOp:
		isPreviousTokenOperand = false;
		*writeFile << "BIT" << " " << maxLengthToken.tokenValue << endl;
		break;
	case ComparisonOp:
		isPreviousTokenOperand = false;
		*writeFile << "COMP" << " " << maxLengthToken.tokenValue << endl;
		break;
	case AssignmentOp:
		isPreviousTokenOperand = false;
		*writeFile << "ASSIGN" << endl;
		break;
	case FloatingPoint:
		isPreviousTokenOperand = true;
		*writeFile << "FLOAT" << " " << maxLengthToken.tokenValue << endl;
		break;
	case LiteralStr:
		isPreviousTokenOperand = false;
		*writeFile << "LITERAL" << " " << maxLengthToken.tokenValue << endl;
		break;
	case SignedInt:
		isPreviousTokenOperand = true;
		*writeFile << "NUM" << " " << maxLengthToken.tokenValue << endl;
		break;
	case ArithmeticOp:
		isPreviousTokenOperand = false;
		*writeFile << "ARITHM" << " " << maxLengthToken.tokenValue << endl;
		break;
	case Comma:
		isPreviousTokenOperand = false;
		*writeFile << "COMMA" << endl;
		break;
	case Brace:
		isPreviousTokenOperand = false;
		*writeFile << "BRACE" << " " << maxLengthToken.tokenValue << endl;
		break;
	case Paren:
		if (maxLengthToken.tokenValue == ")")
			isPreviousTokenOperand = true;
		else
			isPreviousTokenOperand = false;
		*writeFile << "PAREN" << " " << maxLengthToken.tokenValue << endl;
		break;
	case Semicolon:
		isPreviousTokenOperand = false;
		*writeFile << "SEMI" << endl;
		break;
	case Whitespace:
		countNewLine();
	}
}

template <class T1, class T2> // T1 char, CharClass, T2 vector<vector<State>>, vector<Element>
void DfaAccepts(const T1 inputList, const T2 table, const vector<DfaState> finalState, TokenName tokenName) {
	int i = 0;

	DfaState currentState = START_STATE;
	char currentChar;	// 현재 file에서 읽은 character를 저장
	int alphabet;		// currentChar를 index로 쓸 수 있게 integer type으로 alphabet에 저장

	while (currentState != EMPTY) {
		currentChar = getNextChar();
		alphabet = charToIndex(inputList, currentChar);

		lexeme.push_back(currentChar);
		i++;

		currentState = changeState(currentState, alphabet, table); // state transition

		if (inFinal(finalState, currentState) && meetCondition(tokenName, currentChar) && maxLengthToken.maxLength < i) {
			string lexemeToSting(lexeme.begin(), lexeme.end());
			maxLengthToken.maxLength = lexeme.size();
			maxLengthToken.tokenName = tokenName;
			maxLengthToken.tokenValue = lexemeToSting;
		}
	}  // dfa가 reject될 때 or 파일이 끝난 경우 루프 탈출

	lexeme.clear();
	
	readPosition -= i; // file pointer move backward (next to previous token)
}

bool meetCondition(TokenName tokenName, char currentChar) {
	switch (tokenName) {
	case SignedInt: case FloatingPoint:
		if (!isPreviousTokenOperand || lexeme[0] != '-')
			return true;
		else
			return false;
		break;
	default:
		return true;
	}
}

bool inFinal(const vector<DfaState> final, const DfaState previousState) {
	for (unsigned int i = 0; i < final.size(); i++) {
		if (final[i] == previousState)
			return true;
	}
	return false;
}

char getNextChar() { // stream에서 next character 가져오는 함수
	return code[readPosition++];
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

void countNewLine() {
	int i;
	for (i = 0; maxLengthToken.tokenValue[i] != '\0'; i++) {
		if(maxLengthToken.tokenValue[i] == '\n')
			currentLine++;
	}
}