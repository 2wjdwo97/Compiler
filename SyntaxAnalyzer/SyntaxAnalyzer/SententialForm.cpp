#include "SententialForm.h"

SententialForm::SententialForm() {
}
SententialForm::~SententialForm() {
}

void SententialForm::codeToSentence(vector<string> tokens) {
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
}

void SententialForm::pushBack(SYMBOL symbol) {
	sentential.push_back(symbol);
}

vector<SYMBOL> SententialForm::getSentence() {
	return sentential;
}