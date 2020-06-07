#include "SententialForm.h"

SententialForm::SententialForm() {
}
SententialForm::~SententialForm() {
}

/* change the output of lexical_analyzer into the sequence of terminals(sentence) */
void SententialForm::tokensToSentence(const vector<string> tokens) {
	try {
		if (tokens.size() > 2)
			throw Exception("uncorrect symbol table", __FILE__, __LINE__);

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
			throw Exception("uncorrect symbol table", __FILE__, __LINE__);
	}
	catch (Exception e) {
		e.printMessage();
	}
}
/*{
	if (tokens[0].compare("VTYPE") == 0)
		sentential.push_back(SYMBOL::VTYPE);
	else if (tokens[0].compare("NUM") == 0)
		sentential.push_back(SYMBOL::NUM);
	else if (tokens[0].compare("FLOAT") == 0)
		sentential.push_back(SYMBOL::FLOAT);
	else if (tokens[0].compare("LITERAL") == 0)
		sentential.push_back(SYMBOL::LITERAL);
	else if (tokens[0].compare("ID") == 0)
		sentential.push_back(SYMBOL::ID);
	else if (tokens[0].compare("KEYWORD") == 0) {
		if (tokens[1].compare("if") == 0)
			sentential.push_back(SYMBOL::IF);
		else if (tokens[1].compare("else") == 0)
			sentential.push_back(SYMBOL::ELSE);
		else if (tokens[1].compare("while") == 0)
			sentential.push_back(SYMBOL::WHILE);
		else if (tokens[1].compare("for") == 0)
			sentential.push_back(SYMBOL::FOR);
		else if (tokens[1].compare("return") == 0)
			sentential.push_back(SYMBOL::RETURN);
	}
	else if (tokens[0].compare("ARITHM") == 0) {
		if (tokens[1].compare("+") == 0 || tokens[1].compare("-") == 0)
			sentential.push_back(SYMBOL::ADDSUB);
		else if (tokens[1].compare("*") == 0 || tokens[1].compare("/") == 0)
			sentential.push_back(SYMBOL::MULTDIV);
	}
	else if (tokens[0].compare("ASSIGN") == 0)
		sentential.push_back(SYMBOL::ASSIGN);
	else if (tokens[0].compare("COMP") == 0)
		sentential.push_back(SYMBOL::COMP);
	else if (tokens[0].compare("SEMI") == 0)
		sentential.push_back(SYMBOL::SEMI);
	else if (tokens[0].compare("COMMA") == 0)
		sentential.push_back(SYMBOL::COMMA);
	else if (tokens[0].compare("PAREN") == 0) {
		if (tokens[1].compare("(") == 0)
			sentential.push_back(SYMBOL::LPAREN);
		else if (tokens[1].compare(")") == 0)
			sentential.push_back(SYMBOL::RPAREN);
	}
	else if (tokens[0].compare("BRACE") == 0) {
		if (tokens[1].compare("{") == 0)
			sentential.push_back(SYMBOL::LBRACE);
		else if (tokens[1].compare("}") == 0)
			sentential.push_back(SYMBOL::RBRACE);
	}
	else
		cout << "sentence error" << endl;
}*/

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