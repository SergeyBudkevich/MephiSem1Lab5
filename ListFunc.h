#ifndef LISTFUNC_H
#define LISTFUNC_H

#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
	char symb;
	struct ListNode* next;
} ListNode;

typedef struct word_i {
	ListNode* elem_st;
	ListNode* elem_end;
	ListNode* sp_st;
} word_i;

void addListNode(ListNode**, char);
void freeList(ListNode*);
ListNode* splitWord(ListNode*);
void lowerNode(ListNode**);
void uniq(ListNode**);
void uniq_word(ListNode*);
void sort(ListNode**, ListNode*);
int equal_sim(ListNode*, ListNode*);
int count_w(ListNode*);
void append_word(ListNode **head, ListNode **tail, word_i w);
void take_one_space(ListNode **tail, ListNode **spaces);


void minus_tab(ListNode*);
void restr(ListNode**);

#endif
