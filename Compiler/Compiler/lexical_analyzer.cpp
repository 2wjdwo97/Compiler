#include "lexical_analyzer.h"

//----------------------- �������� -----------------------
ifstream readFile;

int currentLine = 1;
vector<char> lexeme;
bool isPreviousTokenOperand = false;

MaxLengthToken maxLengthToken;
vector<ErrorData> errorData;
//---------------------------------------------------------

//----------------------- �Լ� ���� -----------------------
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
		string fileName;
		for (int i = 0; argv[1][i] != '.'; i++)
			fileName.push_back(argv[1][i]);

		ofstream writeFile;
		writeFile.open(fileName + ".out");
		do {
			// DfaAccepts �Լ��� �� ��ū�� Dfa�� ���ؼ� ���ʷ� ȣ��
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
			
			if (maxLengthToken.maxLength == 0) { // accept�� dfa�� �ϳ��� ���ٸ� error�� �Ǵ�
				ErrorData newError;
				newError.line = currentLine;
				newError.wrongInput = readFile.get();
				errorData.push_back(newError);
			}
			else { // accept�� dfa�� �ִٸ� �� token�� ������ ���Ͽ� ����
				WriteToken(&writeFile);
				readFile.seekg(maxLengthToken.maxLength, ios_base::cur);
			}

			// maxLengthToken�� �������� �ʱ�ȭ
			maxLengthToken.maxLength = 0;
			maxLengthToken.tokenName = Null;
			maxLengthToken.tokenValue.clear();
		} while (!readFile.eof()); // EOF�� �ƴ� �� �ݺ�

		// Error ���
		errorData.pop_back(); //������ error�� EOF�̹Ƿ� ����
		for (unsigned int i = 0; i < errorData.size(); i++)
			printf("Error Line: %d, Wrong Input: %c\n", errorData[i].line, errorData[i].wrongInput);

		writeFile.close();
		readFile.close();
	}
}

void WriteToken(ofstream *writeFile) {
	// file write�� �� tokenName�� ���� �� ������ ����
	switch (maxLengthToken.tokenName) {
	case Keyword:
		isPreviousTokenOperand = false;
		*writeFile << "Keyword" << " " << maxLengthToken.tokenValue << endl;
		break;
	case VarType:
		isPreviousTokenOperand = false;
		*writeFile << "VarType" << " " << maxLengthToken.tokenValue << endl;
		break;
	case BooleanStr:
		isPreviousTokenOperand = false;
		*writeFile << "Boolean" << " " << maxLengthToken.tokenValue << endl;
		break;
	case Identifier:
		isPreviousTokenOperand = true;
		*writeFile << "Identifier" << " " << maxLengthToken.tokenValue << endl;
		break;
	case BitwiseOp:
		isPreviousTokenOperand = false;
		*writeFile << "BitwiseOp" << " " << maxLengthToken.tokenValue << endl;
		break;
	case ComparisonOp:
		isPreviousTokenOperand = false;
		*writeFile << "ComparisonOp" << " " << maxLengthToken.tokenValue << endl;
		break;
	case AssignmentOp:
		isPreviousTokenOperand = false;
		*writeFile << "Assignment" << endl;
		break;
	case FloatingPoint:
		isPreviousTokenOperand = true;
		*writeFile << "FloatingPoint" << " " << maxLengthToken.tokenValue << endl;
		break;
	case LiteralStr:
		isPreviousTokenOperand = false;
		*writeFile << "literalStr" << " " << maxLengthToken.tokenValue << endl;
		break;
	case SignedInt:
		isPreviousTokenOperand = true;
		*writeFile << "SignedInt" << " " << maxLengthToken.tokenValue << endl;
		break;
	case ArithmeticOp:
		isPreviousTokenOperand = false;
		*writeFile << "ArithmeticOp" << " " << maxLengthToken.tokenValue << endl;
		break;
	case Comma:
		isPreviousTokenOperand = false;
		*writeFile << "Comma" << endl;
		break;
	case Brace:
		isPreviousTokenOperand = false;
		*writeFile << "Brace" << " " << maxLengthToken.tokenValue << endl;
		break;
	case Paren:
		if (maxLengthToken.tokenValue == ")")
			isPreviousTokenOperand = true;
		else
			isPreviousTokenOperand = false;
		*writeFile << "Paren" << " " << maxLengthToken.tokenValue << endl;
		break;
	case Semicolon:
		isPreviousTokenOperand = false;
		*writeFile << "Semicolon" << endl;
		break;
	case Whitespace:
		countNewLine();
	}
}

template <class T1, class T2> // T1 char, CharClass, T2 vector<vector<State>>, vector<Element>
void DfaAccepts(const T1 inputList, const T2 table, const vector<DfaState> finalState, TokenName tokenName) {
	int i = 0;

	DfaState currentState = START_STATE;
	char currentChar;	// ���� file���� ���� character�� ����
	int alphabet;		// currentChar�� index�� �� �� �ְ� integer type���� alphabet�� ����

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
	}  // dfa�� reject�� �� or ������ ���� ��� ���� Ż��

	lexeme.clear();

	if (currentChar == '\0')	i--;	// EOF�� ��

	readFile.clear();
	if (currentChar == '\n')
		readFile.seekg(-(i + 1), ios_base::cur);
	else
		readFile.seekg(-i, ios_base::cur); // file pointer move backward (next to previous token)
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

char getNextChar() { // stream���� next character �������� �Լ�
	char nextChar;

	if ((nextChar = readFile.get()) != EOF)
		return nextChar;
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

void countNewLine() {
	int i;
	for (i = 0; maxLengthToken.tokenValue[i] != '\0'; i++) {
		if(maxLengthToken.tokenValue[i] == '\n')
			currentLine++;
	}
}