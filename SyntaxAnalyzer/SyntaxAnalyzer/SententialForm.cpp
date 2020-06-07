#include "SententialForm.h"

SententialForm::SententialForm() {
}
SententialForm::~SententialForm() {
}

/* change the output of lexical_analyzer into the sequence of terminals(sentence) */
void SententialForm::tokensToSentence(const vector<string> tokens) {
	Symbol_Info symbol_info;
	
	try {
		if (tokens.size() < 2 || tokens.size() > 3)
			throw Exception("Invalid Symol Table : A line of files(tokenInfo) is not divided into 2 or 3 tokens.", __FILE__, __LINE__);

		if (!isInteger(tokens[0].c_str()))
			throw Exception("Invalid Symol Table : A line number(tokens[0]) is not a number.", __FILE__, __LINE__);

		symbol_info.lineNumber = atoi(tokens[0].c_str());

		unsigned int i;
		for (i = 0; i < table.size(); i++) {
			if (tokens[1].compare(table[i][0].name) == 0) {
				/* Keyword, Arithmetic Op, Paren, Brace */
				if (table[i][0].value == SYMBOL::ERROR) {
					for (unsigned int j = 1; j < table[i].size(); j++) {
						if (table[i][j].name == tokens[2]) {
							sentential.push_back(table[i][j].value);
							symbol_info.tokenValue = tokens[2];
							break;
						}
					}
				}
				else if (table[i][0].value == SYMBOL::ASSIGN) {
					symbol_info.tokenValue = "=";
					sentential.push_back(table[i][0].value);
				}
				else if (table[i][0].value == SYMBOL::SEMI) {
					symbol_info.tokenValue = ";";
					sentential.push_back(table[i][0].value);
				}
				else if (table[i][0].value == SYMBOL::COMMA) {
					symbol_info.tokenValue = ",";
					sentential.push_back(table[i][0].value);
				}
				/* else */
				else{
					symbol_info.tokenValue = tokens[2];
					sentential.push_back(table[i][0].value);
				}
				break;
			}
		}
		errorData.push_back(symbol_info);

		if (i == table.size())
			throw Exception("Invalid Symol Table : The token name(tokens[1]) or token value(tokens[2]) is not properly classified", __FILE__, __LINE__);
	}
	catch (Exception e) {
		e.printMessage();
	}
}

bool SententialForm::isInteger(const string& s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char* p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

void SententialForm::pushBack(const SYMBOL symbol) {
	sentential.push_back(symbol);
}
void SententialForm::pushBackErrorData(const Symbol_Info symbol) {
	errorData.push_back(symbol);
}

void SententialForm::erase(int start, int end) {
	sentential.erase(sentential.begin() + start, sentential.begin() + end);
}

void SententialForm::insert(int index, SYMBOL value) {
	sentential.insert(sentential.begin() + index, value);
}

void SententialForm::eraseErrorData() {
	errorData.erase(errorData.begin());
}

// getter
vector<SYMBOL>& SententialForm::getSentence() {
	return sentential;
}
vector<Symbol_Info> SententialForm::getErrorData() {
	return errorData;
}