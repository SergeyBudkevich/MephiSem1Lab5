#include "ListFunc.h"

//ДОБАВЛЕНИЕ ЭЛЕМЕНТА
void addListNode(ListNode **elem, char symb) {
	ListNode *t = malloc(sizeof(ListNode));
    t->symb = symb;
    t->next = NULL;

    if (*elem == NULL) {
    *elem = t;      
    return;
    }

    ListNode *k = *elem;
    while (k->next != NULL) {
		k = k->next;
    }
    k->next = t;
	return;
}

//ОЧИСТКА ПАМЯТИ ОТ СПИСКА
void freeList(ListNode* elem) { 
	ListNode* t;
	while(elem != NULL) {
	t = elem->next;
    free(elem);
    elem = t;
	}
}

//РАЗДЕЛЕНИЕ СТРОКИ НА СЛОВА
ListNode* splitWord(ListNode* elem) {
	ListNode* c = elem;
	ListNode* word_list = NULL;
	
	while(c) {
		//ПРОПУСКАЕМ ПРОБЕЛЫ И ТАБУЛЯЦИЮ:
		while(c && ((c->symb == ' ') || (c->symb == '\t'))) {
			c = c->next;
		}

		if (c == NULL) {
			return word_list;	
		}
		ListNode* w_st = c;

		//ИЩЕМ КОНЕЦ СЛОВА:	
		while(c && ((c->symb != ' ') && (c->symb != '\t'))) {
			c = c->next;
		}
		ListNode* w_end = c;

		//ЗАПИСЫВАЕМ СЛОВО	
		while(w_st != w_end) {
		addListNode(&word_list, w_st->symb);
		w_st = w_st->next;
		}
		
		if (c != NULL) {
			addListNode(&word_list, ' ');
		}
	}

	return word_list;
}

//ПРИВЕДЕНИЕ ВСЕХ СИМВОЛОВ В НИЖНИЙ РЕГИСТР:
void lowerNode(ListNode** elem) {
	ListNode* c = *elem;
	
	while(c) {
		if ((c->symb >= 'A') && (c->symb <= 'Z')) {
			c->symb = 'a' + (c->symb - 'A');
		} 
		c = c->next;
	}
}

//ОСТАВИТЬ В СЛОВАХ ТОЛЬКО УНИКАЛЬНЫЕ СИМВОЛЫ:
void uniq(ListNode** elem) {
    ListNode* c = *elem;
    ListNode* w_st = *elem;

    while (c) {
        if (c->symb == ' ') {
            if (w_st && w_st != c) {
                uniq_word(w_st);
            }
            w_st = c->next;
        }
        c = c->next;
    }
    if (w_st) {
        uniq_word(w_st);
    }
}

//ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
void uniq_word(ListNode* word) {
    if (!word || word->symb == ' ') {
        return;
    }

    ListNode *space = word;
    while (space && space->symb != ' ') {
        space = space->next;
    }

    int type_char[256] = {0};

    for (ListNode *c = word; c && c != space; c = c->next) {
        type_char[(unsigned char)c->symb] = 1;
    }
    
    ListNode *c = word;
    for (int i = 0; i < 256 && c && c != space; ++i) {
        if (type_char[i]) {
            c->symb = (char)i;
            c = c->next;
        }
    }

    while (c && c != space) {
        c->symb = ' ';      
        c = c->next;
    }
}

//ОСНОВНАЯ ФУНКЦИЯ СОРТИРОВКИ
void sort(ListNode **elem, ListNode *splitW) {
    int n = count_w(splitW);

    word_i *words = malloc(n * sizeof(word_i));
    int *fr = malloc(n * sizeof(int));
    int *us_word = calloc(n, sizeof(int));
    if (!words || !fr || !us_word) {
        free(words); 
        free(fr); 
        free(us_word);
        return;
    }

    //ЗАПОЛНЯЕМ ЗНАЧЕНИЯ
    ListNode *e = *elem;
    ListNode *s = splitW;
    int k = 0;

    while (e && s && k < n) {
        while (e && e->symb == ' ') {
            e = e->next;
        }
        while (s && s->symb == ' ') {
            s = s->next;
        }
        if (!e || !s) {
            break;
        }

        words[k].elem_st = e;
        words[k].sp_st = s;

        while (e && e->symb != ' ') {
            e = e->next;
        }
        while (e && e->symb == ' ') {
            e = e->next;
        }
        words[k].elem_end = e;

        while (s && s->symb != ' ') {
            s = s->next;
        }
        k++;
    }
    n = k;

    //СКОЛЬКО РАЗ ВСТРЕЧАЕТСЯ
    for (int i = 0; i < n; ++i) {
        fr[i] = 0;
        for (int j = 0; j < n; ++j) {
            if (equal_sim(words[i].sp_st, words[j].sp_st)) {
                fr[i]++;
            }
        }
    }

    //ПЕРЕМЕСТИМ ВСЕ ПРОБЕЛЫ ИЗ ELEM В SPACES
    ListNode *spaces = NULL;
    ListNode *spaces_tail = NULL;
    ListNode *c = *elem;
    ListNode *prev = NULL;

    while (c) {
        if (c->symb == ' ') {
            ListNode *sp = c;
            c = c->next;

            if (prev) {
                prev->next = c;
            } else {
                *elem = c;
            }

            sp->next = NULL;
            if (!spaces) {
                spaces = sp;
            } else {
                spaces_tail->next = sp;
            }
            spaces_tail = sp;
        } else {
            prev = c;
            c = c->next;
        }
    }

    //ПЕРЕСОБИРАЕМ СТРОКУ, ВСТАВЛЯЯ ПРОБЕЛЫ
    ListNode *head = NULL; 
    ListNode *tail = NULL;
    int added = 0;

    //СНАЧАЛА ГРУППЫ СЛОВ
    for (int i = 0; i < n; ++i) {
        if (us_word[i]) {
            continue;
        }
        if (fr[i] <= 1) {
            continue;
        }

        append_word(&head, &tail, words[i]);
        us_word[i] = 1;
        added++;
        if (added < n) {
            take_one_space(&tail, &spaces);
        }

        for (int j = i + 1; j < n; ++j) {
            if (!us_word[j] && equal_sim(words[i].sp_st, words[j].sp_st)) {
                append_word(&head, &tail, words[j]);
                us_word[j] = 1;
                added++;
                if (added < n) {
                    take_one_space(&tail, &spaces);
                }
            }
        }
    }

    //ТЕПЕРЬ СЛОВА ПО ОДИНОЧКЕ
    for (int i = 0; i < n; ++i) {
        if (!us_word[i]) {
            append_word(&head, &tail, words[i]);
            us_word[i] = 1;
            added++;
            if (added < n) {
                take_one_space(&tail, &spaces);
            }
        }
    }

    if (tail) {
        tail->next = NULL;
    }
    *elem = head;

    freeList(spaces);
    free(words);
    free(fr);
    free(us_word);
}




//СРАВНЕНИЕ ДВУХ СЛОВ:
int equal_sim(ListNode* word1, ListNode* word2) {
	while(word1 && word2 && word1->symb != ' ' && word2->symb != ' ') {
		if (word1->symb != word2->symb) {
			return 0;
		}
		word1 = word1->next;
		word2 = word2->next;
	}
	if ((word1 == NULL || word1->symb == ' ') && (word2 == NULL || word2->symb == ' ')) {
		return 1;
	} else {
		return 0;
	}
}

//СЧИТАЕМ КОЛ-ВО СЛОВ:
int count_w(ListNode* s) {
	int n = 0;
	int in_w = 0;

	while(s) {
		if (s->symb == ' ') {
			in_w = 0;
		} else if (in_w == 0) {
			in_w = 1;
			n++;
		}
		s = s->next;
	}
	return n;
}

void append_word(ListNode **head, ListNode **tail, word_i w) {
    if (!*head) {
        *head = w.elem_st;
    } else {
        (*tail)->next = w.elem_st;
    }

    ListNode *t = w.elem_st;
    while (t->next != w.elem_end) {
        t = t->next;
    }
    *tail = t;
    (*tail)->next = NULL;
}

void take_one_space(ListNode **tail, ListNode **spaces) {
    if (!tail || !*tail) {
        return;
    }
    if (!spaces || !*spaces) {
        return;
    }

    (*tail)->next = *spaces;
    *tail = *spaces;
    *spaces = (*spaces)->next;
    (*tail)->next = NULL;
}


void minus_tab(ListNode *elem) {
    if (!elem) {
        return;
    }

    ListNode* c = elem;

    while (c) {
        if (c->symb == '\t') {
            c->symb = ' ';
        }
        c = c->next;
    }

}
