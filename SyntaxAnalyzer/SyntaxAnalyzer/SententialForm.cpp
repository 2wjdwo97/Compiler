#include "SententialForm.h"

SententialForm::SententialForm() {
}
SententialForm::~SententialForm() {
}

/* change the output of lexical_analyzer into the sequence of terminals(sentence) */
void SententialForm::tokensToSentence(const vector<string> tokens) {
	try {
		if (tokens.size() < 1 || tokens.size() > 2)
			throw Exception("Invalid Symol Table : A line of files is not divided into one or two tokens.", __FILE__, __LINE__);

		unsigned int i;
		for (i = 0; i < table.size(); i++) {
			if (tokens[0].compare(table[i][0].name) == 0) {
				/* Keyword, Arithmetic Op, Paren, Brace */
				if (table[i][0].value == SYMBOL::ERROR) {
					for (unsigned int j = 1; j < table[i].size(); j++) {
						if (table[i][j].name == tokens[1]) {
							sentential.push_back(table[i][j].value);
							break;
						}
					}
				}
				/* else */
				else {
					sentential.push_back(table[i][0].value);
				}
				break;
			}
		}

		if (i == table.size())
			throw Exception("Invalid Symol Table : TokenName cannot be classified", __FILE__, __LINE__);
	}
	catch (Exception e) {
		e.printMessage();
	}
}

void SententialForm::pushBack(const SYMBOL symbol) {
	sentential.push_back(symbol);
}

void SententialForm::erase(int start, int end) {
	sentential.erase(sentential.begin() + start, sentential.begin() + end);
}

void SententialForm::insert(int index, SYMBOL value) {
	sentential.insert(sentential.begin() + index, value);
}

vector<SYMBOL>& SententialForm::getSentence() {
	return sentential;
}