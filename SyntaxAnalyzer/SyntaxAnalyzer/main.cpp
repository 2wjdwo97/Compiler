#include "main.h"

int main(int argc, char* argv[]) {

	ifstream readFile;
	SententialForm sentence;

	try {
		if (argc != 2)
			throw Exception("The execution command of the syntax analyzer: syntax_analyzer <input_file_name.out> ");

		//readFile.open("test.out");
		readFile.open(argv[1]);
	
		if (readFile.is_open())
		{
			SLRparser parser;

			/* change the output of lexical_analyzer into the sequence of terminals(sentence) */
			string line;

			while (!readFile.eof()) {
				getline(readFile, line);
				vector<string> tokens = split(line, '@');
				sentence.tokensToSentence(tokens);
			}

			/* Add the end part of sentence and error data */
			sentence.pushBack(SYMBOL::ENDMARKER);

			SymbolErrorInfo endErrorData;
			endErrorData.lineNumber = sentence.getErrorData()[sentence.getErrorData().size() - 1].lineNumber;
			endErrorData.tokenValue = "$";
			sentence.pushBackErrorData(endErrorData);

			/* SLR parsing */
			parser.SLRparsing(sentence);

			/* print a result */
			if (parser.isAccept(sentence))
				cout << "ACCEPT : correct sentence" << endl;
			else
				cout << "CANNOT ACCEPT : uncorrenct sentence" << endl;
		}
		else
			throw Exception("Cannot open file : ", argv[1]);
	}
	catch (Exception e) {
		/* print error line and unexpected value */
		if (sentence.getErrorData().size()) {
			cout << "Error Line: " << sentence.getErrorData()[0].lineNumber << endl;
			cout << "Unexpected Value: " << sentence.getErrorData()[0].tokenValue << endl;
		}
		e.printMessage();
	}

	readFile.close();
	return 0;
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
