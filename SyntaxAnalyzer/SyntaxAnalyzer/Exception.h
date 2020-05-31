#pragma once
#include <iostream>
using namespace std;

class Exception
{
private:
	string message;
	string fileName;
	int lineNum;

public:
	Exception(string, string);
	Exception(string, string, int);
	~Exception();
	void printMessage();
};