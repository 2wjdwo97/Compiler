#include "main.h"

int main(int argc, char* argv[]) {
	//if (argc != 2) {
	//	cout << "The execution command of the syntax analyzer: syntax_analyzer <input_file_name> \n";
	//	return;
	//}
	//readFile.open(argv[1]);

	ifstream readFile;
	readFile.open("test.out");

	try {
		if (readFile.is_open())
		{
			SententialForm sentence;
			SLRparser parser;

			/* change the output of lexical_analyzer into the sequence of terminals(sentence) */
			string line;

			while (!readFile.eof()) {
				getline(readFile, line);
				vector<string> tokens = split(line, ' ');
				sentence.tokensToSentence(tokens);
			}

			sentence.pushBack(SYMBOL::ENDMARKER);

			/* SLR parsing */
			parser.SLRparsing(sentence);

			/* print result */
			if (parser.isAccept(sentence))
				cout << "ACCEPT : correct sentence" << endl;
			else
				cout << "CANNOT ACCEPT : uncorrenct sentence" << endl;
		}
		else
			throw Exception("Cannot open file : ", argv[1]);
	}
	catch (Exception e) {
		e.printMessage();
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
