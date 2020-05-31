#include "SLRparser.h"

/* initialization */
/* Push start state(1) into the stack */
/*  - Current state = the stored in the top of the stack */
/*  - Next input symbol = the leftmost terminal of a right substring */
SLRparser::SLRparser() {
	st.push(1);	
	splitter = 0;
	nextInputSymbol = SYMBOL::ENDMARKER;
	isFinish = false;
}
SLRparser::~SLRparser() {
}

void SLRparser::parsing(vector<SYMBOL> sententialForm) {
	sentential = sententialForm;
	nextInputSymbol = sentential[0];

	while (!isFinish) {
		changeState(st.top() - 1, toIndex(nextInputSymbol, inputSymbolList_Action));

		if (checkAccept())	break;
	}
}

template <class T>
int SLRparser::toIndex(T symbol, vector<T> inputList) {
	try {
		for (int i = 0; i < inputList.size(); i++) {
			if (inputList[i] == symbol)
				return i;	// return index
		}
		throw Exception("uncorrect input symbol", __FILE__, __LINE__);
	}
	catch (Exception e) {
		e.printMessage();
	}
}

void SLRparser::changeState(int currentState, int input) {
	switch (SLRtable_Action[currentState][input].action) {
	/* Make a decision : Shift */
	case ACTION::SHIFT:		
		st.push(SLRtable_Action[currentState][input].actionNum);	// push the next state into the stack
		nextInputSymbol = sentential[++splitter];					// move the splitter to the right, change the next input sumbol
		break;

	/* Make a decision : Reduce */
	case ACTION::REDUCE:	
		/* for A -> ес, pop |ес| contents from the stack */
		int i;
		for (i = 0; i < CFG[SLRtable_Action[currentState][input].actionNum - 1].length; i++)
			st.pop();
		/* reduce by corresponding production */
		sentential.erase(sentential.begin() + splitter - i, sentential.begin() + splitter);		
		splitter -= i;
		sentential.insert(sentential.begin() + splitter, CFG[SLRtable_Action[currentState][input].actionNum - 1].symbol);
		splitter++;
		/* for A -> ес, push GOTO (currentState, A) into the stack */
		st.push(SLRtable_Goto[st.top() - 1][toIndex(CFG[SLRtable_Action[currentState][input].actionNum - 1].symbol, inputSymbolList_Goto)]);
		if (st.top() == 0 && !checkAccept())
			isFinish = true;
		break;

	/* Error */
	case ACTION::EMPTY:
		isFinish = true;
		break;
	}
}

bool SLRparser::checkAccept() {
	if (sentential[0] == SYMBOL::START && sentential[1] == SYMBOL::ENDMARKER)
		return true;
	else
		return false;
}

bool SLRparser::getIsFinish() {
	return isFinish;
}