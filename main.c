#include "ListFunc.h"

int main(void) {
    int c;                
	ListNode *elem = NULL;

    while (1) {             
        printf("Enter your line -> ");
        elem = NULL;

        while ((c = getchar()) != '\n' && c != EOF) {
            addListNode(&elem, c);
        }

        if (c == EOF) {   
            freeList(elem);
            elem = NULL;
			printf("\nGoodbye\n");
            break;         
        }

        ListNode *t = elem;
		printf("Your str: \"");
        while (t != NULL) {
            printf("%c", t->symb);
            t = t->next;
        }
        printf("\"\n");

		restr(&elem);
		t = elem;

		printf("Restr: \"");
		while (t) {
			printf("%c", t->symb);
			t = t->next;
		}
		printf("\"\n");


        freeList(elem);
        elem = NULL;
    }
    return 0;
}

