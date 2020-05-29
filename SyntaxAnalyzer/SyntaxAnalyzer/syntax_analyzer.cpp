#include "syntax_analyzer.h"

int main(int argc, char* argv[]) {
	//if (argc != 2) {
	//	cout << "The execution command of the syntax analyzer: syntax_analyzer <input_file_name> \n";
	//	return;
	//}

	//readFile.open(argv[1]);
	
	readFile.open("test.out");

	if (readFile.is_open())
	{
		string line;
		while (!readFile.eof()) {
			getline(readFile, line);
			vector<string> tokens = split(line, ' ');
			codeToSentence(tokens);
		}
		sentinelForm.push_back(SYMBOL::ENDMARKER);

		if (SLRparsing())
			cout << "correct sentence" << endl;
		else
			cout << "error : uncorrenct sentence" << endl;
	}
	else {
		cout << "Error: Cannot open file : " << argv[1];
		exit(0);
	}

	readFile.close();
}

vector<string> split(const string str, const char delimiter) {
	vector<string> tokens;
	stringstream ss(str);
	string temp;

	while (getline(ss, temp, delimiter)) {
		tokens.push_back(temp);
	}
	return tokens;
}

void codeToSentence(vector<string> tokens) {
	if (tokens[0].compare("VTYPE") == 0)
		sentinelForm.push_back(SYMBOL::VTYPE);
	else if (tokens[0].compare("NUM") == 0)
		sentinelForm.push_back(SYMBOL::NUM);
	else if (tokens[0].compare("FLOAT") == 0)
		sentinelForm.push_back(SYMBOL::FLOAT);
	else if (tokens[0].compare("LITERAL") == 0)
		sentinelForm.push_back(SYMBOL::LITERAL);
	else if (tokens[0].compare("ID") == 0)
		sentinelForm.push_back(SYMBOL::ID);
	else if (tokens[0].compare("KEYWORD") == 0) {
		if (tokens[1].compare("if") == 0)
			sentinelForm.push_back(SYMBOL::IF);
		else if (tokens[1].compare("else") == 0)
			sentinelForm.push_back(SYMBOL::ELSE);
		else if (tokens[1].compare("while") == 0)
			sentinelForm.push_back(SYMBOL::WHILE);
		else if (tokens[1].compare("for") == 0)
			sentinelForm.push_back(SYMBOL::FOR);
		else if (tokens[1].compare("return") == 0)
			sentinelForm.push_back(SYMBOL::RETURN);
	}
	else if (tokens[0].compare("ARITHM") == 0) {
		if (tokens[1].compare("+") == 0 || tokens[1].compare("-") == 0)
			sentinelForm.push_back(SYMBOL::ADDSUB);
		else if (tokens[1].compare("*") == 0 || tokens[1].compare("/") == 0)
			sentinelForm.push_back(SYMBOL::MULTDIV);
	}
	else if (tokens[0].compare("ASSIGN") == 0)
		sentinelForm.push_back(SYMBOL::ASSIGN);
	else if (tokens[0].compare("COMP") == 0)
		sentinelForm.push_back(SYMBOL::COMP);
	else if (tokens[0].compare("SEMI") == 0)
		sentinelForm.push_back(SYMBOL::SEMI);
	else if (tokens[0].compare("COMMA") == 0)
		sentinelForm.push_back(SYMBOL::COMMA);
	else if (tokens[0].compare("PAREN") == 0) {
		if (tokens[1].compare("(") == 0)
			sentinelForm.push_back(SYMBOL::LPAREN);
		else if (tokens[1].compare(")") == 0)
			sentinelForm.push_back(SYMBOL::RPAREN);
	}
	else if (tokens[0].compare("BRACE") == 0) {
		if (tokens[1].compare("{") == 0)
			sentinelForm.push_back(SYMBOL::LBRACE);
		else if (tokens[1].compare("}") == 0)
			sentinelForm.push_back(SYMBOL::RBRACE);
	}
	else
		cout << "sentence error" << endl;
}

bool SLRparsing() {
	nextInputSymbol = sentinelForm[0];
	st.push(1);

	while(!isError) {
		changeState(st.top()-1, toIndex(nextInputSymbol, inputSymbolList_Action));

		if (sentinelForm[0] == SYMBOL::START && sentinelForm[1] == SYMBOL::ENDMARKER)
			return true;
	}
	return false;
}

template <class T>
int toIndex(T symbol, vector<T> inputList) {
	for (int i = 0; i < inputList.size(); i++) {
		if (inputList[i] == symbol)
			return i;	// return index
	}
	return -1;			//error
}

void changeState(int currentState, int input) {
	switch (SLRtable_Action[currentState][input].action) {
	case ACTION::SHIFT:
		st.push(SLRtable_Action[currentState][input].actionNum);
		nextInputSymbol = sentinelForm[++splitter];
		break;
	case ACTION::REDUCE:
		int i;
		for (i = 0; i < CFG[SLRtable_Action[currentState][input].actionNum - 1].length; i++)
			st.pop();
		sentinelForm.erase(sentinelForm.begin() + splitter - i, sentinelForm.begin() + splitter);
		splitter -= i;
		sentinelForm.insert(sentinelForm.begin() + splitter, CFG[SLRtable_Action[currentState][input].actionNum - 1].symbol);
		splitter++;
		st.push(SLRtable_Goto[st.top() - 1][toIndex(CFG[SLRtable_Action[currentState][input].actionNum - 1].symbol, inputSymbolList_Goto)]);
		if (st.top() == 0)
			isError = true;
		break;
	case ACTION::EMPTY:
		isError = true;
		break;
	}
}