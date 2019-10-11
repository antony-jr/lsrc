#ifndef CSTR_LIST_H_INCLUDED
#define CSTR_LIST_H_INCLUDED
#include <cstr.h>
#include <stdlib.h>

struct _cstr_list_t;
typedef struct _cstr_list_t* cstr_list_t;

void *cstr_list_end();

cstr_list_t cstr_list_new_ex();
cstr_list_t cstr_list_new(cstr_t,...);
void cstr_list_free(cstr_list_t, /*free resources for listed cstrs=*/int);

size_t cstr_list_length(cstr_list_t);
cstr_t cstr_list_get(cstr_list_t, size_t);
int cstr_list_set(cstr_list_t, size_t, cstr_t);

int cstr_list_append(cstr_list_t, ...);


#endif /* CSTR_LIST_H_INCLUDED */
