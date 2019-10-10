#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstr.h>
#include <cw.h>
#include <fwriter.h>

struct _fwriter_t {
	FILE *file,
	     *license;
	int type;
	int is_replace;
	cstr_t buffer;
	cstr_t out_file_name;
};
#define FWRITER_SZ (sizeof(struct _fwriter_t))

static cstr_t get_line(FILE *fp){
	cstr_t r = cstr_new();
	int c = 0;
	while((c = getc(fp)) != EOF){
		if(cstr_append_char(r, c)){
			cstr_free(r);
			return NULL;
		}
		if(c == '\n'){
			break;
		}
	}
	if(c == EOF){
		cstr_free(r);
		return NULL;
	}
	return r;
}

fwriter_t fwriter_new(const char *license_file_path,
		      const char *file_path,
		      int type,
		      int replace){
	fwriter_t r = calloc(1,FWRITER_SZ);
	if(r == NULL)
		return NULL;
	r->is_replace = replace;
	r->file = fopen(file_path, "r");
	if(!r->file){
		fwriter_free(r);
		return NULL;
	}
	r->license = fopen(license_file_path,"r");
	if(!r->license){
		fwriter_free(r);
		return NULL;
	}
	r->out_file_name = cstr_new_ex(file_path);
	if(r->out_file_name == NULL){
		fwriter_free(r);
		return NULL;
	}
	r->type = type;	
	switch(type){
		case PYTHON_FILE:
		default:
			break;
	}
	return r;
}

void fwriter_free(fwriter_t obj){
	if(obj == NULL)
		return;
	if(obj->file)
		fclose(obj->file);
	if(obj->license)
		fclose(obj->license);
	cstr_free(obj->buffer);
	cstr_free(obj->out_file_name);
	free(obj);
}

int fwriter_exec(fwriter_t obj){	
	cstr_t line = NULL;
	cstr_t path = NULL;
	int c = 0;
	FILE *fp = NULL;
	cw_t *writer = NULL;
	const char *s = NULL;
	const char *start = NULL,
	           *end = NULL;
	if(obj == NULL)
		return -1;
	
	if(obj->type == C_FILE){
		start = "/*\n";
		end = "*/\n";
		writer = cw_new("/*\n", " * ", "*/\n");
	}else if(obj->type == PYTHON_FILE){
		start = "# ";
		writer = cw_new(NULL, "# ", NULL);
	}

	if(writer == NULL)
		return -1;

	rewind(obj->file);
	rewind(obj->license);

	/* copy original file somewhere. */
	path = cstr_new_ex(cstr_digest(obj->out_file_name));
	cstr_append(path, ".orig");
	if(!(fp = fopen(cstr_digest(path), "w"))){
		cw_free(writer);
		cstr_free(path);
		return -1;
	}
	cstr_free(path);
	while((c = getc(obj->file)) != EOF)
		putc(c, fp);
	fclose(fp);
	rewind(obj->file);

	if(obj->is_replace){
		if(obj->type == C_FILE){
			line = get_line(obj->file);
			if(strcmp(cstr_digest(line), start)){
				cstr_free(line);
				rewind(obj->file);
			}else{
				while(strcmp(cstr_digest(line), end)){
					cstr_free(line);
					line = get_line(obj->file);	
				}
				cstr_free(line);
			}
		}
		/* TODO: add more source file support. */
	}

	cw_start(writer);
	while((line = get_line(obj->license)) != NULL){
		cw_write_line(writer, cstr_digest(line));	
		cstr_free(line);
	}
	cw_end(writer);

	s = cw_digest(writer);

	if(!(fp = fopen(cstr_digest(obj->out_file_name), "w"))){
		cw_free(writer);
		return -1;
	}
	
	/* first copy the copyright stuff from the writer. */
	while(*s)
		putc(*s++, fp);

	/* simply copy the contents of the file.*/
	while((c = getc(obj->file)) != EOF)
		putc(c, fp);

	fclose(fp);
	cw_free(writer);
	return 0;
}
