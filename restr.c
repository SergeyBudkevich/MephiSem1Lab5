#include "ListFunc.h"

void restr(ListNode** elem) {
	ListNode* splitW;

    minus_tab(*elem);
	splitW = splitWord(*elem);
	lowerNode(&splitW);
	uniq(&splitW);

	sort(elem, splitW);
	freeList(splitW);
	return;
}

