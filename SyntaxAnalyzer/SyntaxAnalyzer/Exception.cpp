#include "Exception.h"

Exception::Exception(string msg) {
	cout << msg << endl;
	exit(0);
}

Exception::Exception(string msg, string file) {
	message = msg;
	fileName = file;
	lineNum = 0;
}

Exception::Exception(string msg, string file, int line) {
	message = msg;
	fileName = file;
	lineNum = line;
}

Exception::~Exception() {
}

void Exception::printMessage() {
	cout << message << endl << fileName;
	if (lineNum)
		cout << "\\line" << lineNum;
	cout << endl;
	exit(0);
}