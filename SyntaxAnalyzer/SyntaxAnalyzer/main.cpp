#include "main.h"

int main(int argc, char* argv[]) {

	ifstream readFile;
	//if (argc != 2) {
	//	cout << "The execution command of the syntax analyzer: syntax_analyzer <input_file_name> \n";
	//	return;
	//}

	//readFile.open(argv[1]);


	readFile.open("test.out");

	if (readFile.is_open())
	{
		SententialForm sentence;
		SLRparser parser;

		/* change the output of lexical_analyzer into the sequence of terminals(sentence) */
		string line;

		while (!readFile.eof()) {
			getline(readFile, line);
			vector<string> tokens = split(line, ' ');
			sentence.codeToSentence(tokens);
		}
		sentence.pushBack(SYMBOL::ENDMARKER);

		/* SLR parsing */
		parser.parsing(sentence.getSentence());

		/* print result */
		if (!parser.getError())
			cout << "accept : correct sentence" << endl;
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
