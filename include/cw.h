#ifndef CW_H_INCLUDED
#define CW_H_INCLUDED

struct _cw_t;
typedef struct _cw_t cw_t;

cw_t *cw_new(/*what to write on start of multiline comment=*/const char *, 
	     /*what to write on each line of comment=*/const char *, 
	     /*what to write on finish of the multiline comment=*/const char *);
void cw_free(cw_t*);

int cw_start(cw_t*);
int cw_end(cw_t*);
int cw_write_line(cw_t*,const char *);
int cw_write_char(cw_t*,char c);

char *cw_get_string(cw_t*);

#endif /* CW_H_INCLUDED */
