#ifndef GOSORT
#define GOSORT

typedef int go_compareFunction(void *, void *);

int go_asc_compare(void *A, void *B);
int go_desc_compare(void *A, void *B);

#define ASC_SORT go_asc_compare
#define DESC_SORT go_desc_compare

void go_sort(void *tab, int len);

void go_sort_init(go_compareFunction fct, int data_size);

#endif
