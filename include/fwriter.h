#ifndef FWRITER_H_INCLUDED
#define FWRITER_H_INCLUDED
#define C_FILE 0
#define PYTHON_FILE 1

struct _fwriter_t;
typedef struct _fwriter_t* fwriter_t;

fwriter_t fwriter_new(/*license file to insert=*/const char*,
		      /*file to insert=*/const char*, 
		      /*type of comments to use=*/int,
		      /*replace top multi-line comment=*/int,
		      /*do in place write=*/int,
		      /*output file name(default=stdout)=*/const char*);
void fwriter_free(fwriter_t);

int fwriter_exec(fwriter_t);

#endif /* FWRITER_H_INCLUDED */
