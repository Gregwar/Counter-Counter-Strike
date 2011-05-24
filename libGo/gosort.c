#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gosort.h>

go_compareFunction *heap_compare;
int heap_data_size;

int go_asc_compare(void *A, void *B) {
	return ((*(int *)A)<(*(int *)B));
}

int go_desc_compare(void *A, void *B) {
	return ((*(int *)A)>(*(int *)B));
}

inline void go_swap(void *A, void *B) {
	char tmp[heap_data_size];
	memcpy(tmp,A,heap_data_size);
	memcpy(A,B,heap_data_size);
	memcpy(B,tmp,heap_data_size);
}

void go_tamise(void *tab, int noeud, int len) {
	int fils = (2*(noeud+1)) -1;
	while (fils<len) {
		if (fils<len-1 && heap_compare((tab+heap_data_size*fils),(tab+heap_data_size*(fils+1)))) {
			fils++;
		}
		if (heap_compare((tab+heap_data_size*noeud),(tab+heap_data_size*fils))) {
			go_swap((tab+heap_data_size*noeud), (tab+heap_data_size*fils));
		} else {
			break;
		}
		noeud = fils;
		fils = (2*(noeud+1)) -1;
	}

}

void go_sort(void *tab, int len) {
	int i;
	for (i=len-1; i>=0; i--) {
		go_tamise(tab, i, len);
	}
	for (i=len-1; i>=0; i--) {
		go_swap(tab, (tab+i*heap_data_size));
		go_tamise(tab, 0, i);
	}
}

void go_sort_init(go_compareFunction fct, int data_size) {
	heap_data_size=data_size;
	heap_compare=fct;
}
