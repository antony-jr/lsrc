#ifndef CSTR_H_INCLUDED
#define CSTR_H_INCLUDED

struct _cstr_t;
typedef struct _cstr_t* cstr_t;

cstr_t cstr_new();
cstr_t cstr_new_ex(const char*);
void cstr_free(cstr_t);

int cstr_prepend_char(cstr_t,char);
int cstr_append_char(cstr_t,char);
int cstr_prepend(cstr_t, const char*);
int cstr_prepend_ex(cstr_t, cstr_t);
int cstr_append(cstr_t, const char*);
int cstr_append_ex(cstr_t,cstr_t);

const char *cstr_digest(cstr_t);

#endif /* CSTR_H_INCLUDED */
