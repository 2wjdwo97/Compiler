#include "SLRparser.h"

/* initialization */
/* Push start state(1) into the stack */
/*  - Current state = the stored in the top of the stack */
/*  - Next input symbol = the leftmost terminal of a right substring */
SLRparser::SLRparser() {
	st.push(1);	
	splitter = 0;
	nextInputSymbol = SYMBOL::ENDMARKER;
}
SLRparser::~SLRparser() {
}

void SLRparser::SLRparsing(SententialForm& sententialForm) {
	nextInputSymbol = sententialForm.getSentence()[0];

	do {
		changeState(sententialForm, st.top() - 1, nextInputSymbol);

	} while (!isAccept(sententialForm));
}

void SLRparser::changeState(SententialForm& sententialForm, const int currentState, const SYMBOL input) {
	State state = SLRtable_Action[getIndex(currentState, input, SLRtable_Action)].action;

	try {
		switch (state.action) {
		/* Make a decision : Shift */
		case ACTION::SHIFT:
			st.push(state.actionNum);										// push the next state into the stack
			nextInputSymbol = sententialForm.getSentence()[++splitter];		// move the splitter to the right, change the next input sumbol
			break;

		/* Make a decision : Reduce */
		case ACTION::REDUCE:
			/* for A -> ес, pop |ес| contents from the stack */
			int i;
			for (i = 0; i < CFG[state.actionNum - 1].length; i++)
				st.pop();

			/* do a reduction of the suffix (right end substring) of the left substring which matches the RHS of a production */
			if (i == 0) {
				sententialForm.insert(splitter++, CFG[state.actionNum - 1].symbol);
			}
			else {
				sententialForm.erase(splitter - i, splitter);
				splitter -= i;
				sententialForm.insert(splitter, CFG[state.actionNum - 1].symbol);
				splitter++;
			}

			/* for A -> ес, push GOTO (currentState, A) into the stack */
			st.push(SLRtable_Goto[getIndex(st.top() - 1, CFG[state.actionNum - 1].symbol, SLRtable_Goto)].stateNum);
			break;

		/* Error */
		default:
			throw Exception("CANNOT ACCEPT : ", __FILE__, __LINE__);
		}
	}
	catch (Exception e) {
		e.printMessage();
	}
}

template <class T>
int SLRparser::getIndex(const int currentState, const SYMBOL inputIndex, const vector<T> SLRtable)
{
	int index = binarySearch(currentState, SLRtable);

	if (index == -1)
		throw Exception("CANNOT ACCEPT : No matching ROW found in the sparse matrix. ", __FILE__, __LINE__);
	else
	{
		int i;
		for (i = index; i >= 0; i--) {
			if (SLRtable[i].row != currentState)
				break;
			else if (SLRtable[i].column == inputIndex)
				return i;
		}
		for (unsigned int i = index; i < SLRtable.size(); i++) {
			if (SLRtable[i].row != currentState)
				break;
			else if (SLRtable[i].column == inputIndex)
				return i;
		}
		throw Exception("CANNOT ACCEPT : No matching COLUMN found in the sparse matrix. ", __FILE__, __LINE__);
	}
}

template <class T>
int SLRparser::binarySearch(const int state, vector<T> SLRtable)
{
	int low = 0, mid, high = SLRtable.size() - 1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (state < SLRtable[mid].row)
			high = mid - 1;
		else if (state > SLRtable[mid].row)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}

bool SLRparser::isAccept(SententialForm &sententialForm) {
	if (sententialForm.getSentence()[0] == SYMBOL::START && sententialForm.getSentence()[1] == SYMBOL::ENDMARKER && sententialForm.getSentence().size() == 2)
		return true;
	else
		return false;
}